// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#define KB_TOOL_ONLY

#include <kb/core.h>
#include <kb/time.h>
#include <kb/texture.h>
#include <kb/font.h>
#include <kb/log.h>

#include <kbextra/cliargs.h>

#include "kb/alloc.cpp"
#include "kb/crt.cpp"
#include "kb/log.cpp"
#include "kb/thread.cpp"
#include "kb/font.cpp"
#include "kb/table.cpp"
#include "kbextra/cliargs.cpp"

#include "platform/platform_rwops_sdl.cpp"

#define STB_RECT_PACK_IMPLEMENTATION 
#include "stb_rect_pack.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION 
#include "stb_truetype.h"

#define EXIT_FAIL     1
#define EXIT_SUCCESS  0

const uint32_t  font_padding          = 8;
const uint32_t  font_on_edge_value    = 128;
const float     font_pixel_dist_scale = 64.0f;
const float     font_size             = 128.0f;
const uint32_t  range_start           = 0x0000;
const uint32_t  range_end             = 0x007F;
const uint32_t  range_count           = range_end - range_start;
const uint32_t  extra_padding         = 0;
const uint32_t  channels              = 4;

void write_func(void* user, void* data, int size) {
  kb_rwops* io = (kb_rwops*) user;
  kb_rwops_write(io, data, size);
}

void print_help(const char* error = nullptr) {
  if (error != nullptr) printf("%s\n", error);

  printf(
    "Kimberlite fontc\n"
    "\tUsage: fontc --input <in> --output <out>\n"
  );
}

int main(int argc, const char* argv[]) {
  int exit_val = EXIT_FAIL;
  
  kb_rwops*          rwops_in          = nullptr;
  kb_rwops*          rwops_out         = nullptr;
  const char*     in_filepath       = nullptr;
  const char*     out_filepath      = nullptr;

  kb_font_data font {};

  kb_cli_args args {};
  kb_cliargs_init(&args, argc, argv);

  if (kb_cliargs_has(&args, "help")) {
    print_help();
    goto end;
  }

  kb_cliargs_get_str(&args, &in_filepath, "input");
  if (in_filepath == nullptr) {
    print_help("Please specify input file with --input");
    goto end;
  }

  kb_cliargs_get_str(&args, &out_filepath, "output");
  if (out_filepath == nullptr) {
    print_help("Please specify output file with --output");
    goto end;
  }

  rwops_in = kb_rwops_open_file(in_filepath, KB_FILE_MODE_READ);
  if (!rwops_in) {
    print_help("Unable to open input file");
    goto end;
  }

  rwops_out = kb_rwops_open_file(out_filepath, KB_FILE_MODE_WRITE);
  if (!rwops_out) {
    print_help("Unable to open output file");
    goto end;
  }

  // Load (modified from https://github.com/raysan5/raylib/blob/master/src/text.c#L568)
  {  
    unsigned char** fontchar_data = KB_DEFAULT_ALLOC_TYPE(unsigned char*, range_count);

    uint64_t font_data_size = kb_rwops_size(rwops_in);
    unsigned char* font_data = (unsigned char*) KB_DEFAULT_ALLOC(font_data_size);
    kb_rwops_read(rwops_in, font_data, font_data_size);

    stbtt_fontinfo font_info;
    stbtt_InitFont(&font_info, font_data, stbtt_GetFontOffsetForIndex(font_data, 0));
    
    float scale_factor = stbtt_ScaleForPixelHeight(&font_info, font_size);
    
    font.info.pixel_height = font_size;
    font.info.scale_factor = scale_factor;
    font.info.char_count = range_count;
    font.info.chars = KB_DEFAULT_ALLOC_TYPE(kb_font_char, range_count);
    
    kb_log_info("kb_font_data scale factor {}", font.info.scale_factor);

    stbtt_GetFontVMetrics(&font_info, &font.info.ascent, &font.info.descent, &font.info.line_gap);

    // Get char info
    for (uint32_t i = 0; i < font.info.char_count; i++) {
      int codepoint = range_start + i;

      font.info.chars[i].codepoint = codepoint;

      fontchar_data[i] = stbtt_GetCodepointSDF(
        &font_info, scale_factor, codepoint, 
        font_padding, font_on_edge_value, font_pixel_dist_scale, 
        &font.info.chars[i].rect.size.x, &font.info.chars[i].rect.size.y, 
        &font.info.chars[i].rect.from.x, &font.info.chars[i].rect.from.y
      );
      
      stbtt_GetCodepointHMetrics(&font_info, codepoint, &font.info.chars[i].advance, &font.info.chars[i].left_bearing);
        
      stbtt_GetCodepointBitmapBox(&font_info, codepoint, scale_factor, scale_factor, 
        &font.info.chars[i].bbox_from.x, &font.info.chars[i].bbox_from.y, 
        &font.info.chars[i].bbox_to.x, &font.info.chars[i].bbox_to.y
      );
      
      font.info.chars[i].advance *= scale_factor;
    }

    // Pack to atlas
    float required_area = 0;

    for (uint32_t i = 0; i < range_count; i++) {
      if (fontchar_data[i] == NULL) continue;
      required_area += ((font.info.chars[i].rect.size.x + 2 * extra_padding) * (font.info.chars[i].rect.size.y + 2 * extra_padding));
    }

    float guess_size = sqrtf(required_area);
    int image_size = (int) powf(2, ceilf(logf((float)guess_size) / logf(2)));
    kb_log_info("required_area {} image_size {}", required_area, image_size);

    font.atlas_bitmap_width  = image_size;
    font.atlas_bitmap_height = image_size;
    
    font.atlas_bitmap_size = font.atlas_bitmap_width * font.atlas_bitmap_height * channels;
    font.atlas_bitmap = KB_DEFAULT_ALLOC(font.atlas_bitmap_size);
    kb_memset(font.atlas_bitmap, 0, font.atlas_bitmap_size);

    stbrp_context*  context  = KB_DEFAULT_ALLOC_TYPE(stbrp_context, 1);
    stbrp_node*     nodes    = KB_DEFAULT_ALLOC_TYPE(stbrp_node, range_count);
    stbrp_rect*     rects    = KB_DEFAULT_ALLOC_TYPE(stbrp_rect, range_count);

    kb_log_info("W {} H {}", font.atlas_bitmap_width, font.atlas_bitmap_height);
    stbrp_init_target(context, font.atlas_bitmap_width, font.atlas_bitmap_height, nodes, range_count);
    
    for (uint32_t i = 0; i < range_count; i++) {
      rects[i].id = i;
      rects[i].w = font.info.chars[i].rect.size.x + 2 * extra_padding;
      rects[i].h = font.info.chars[i].rect.size.y + 2 * extra_padding;
    }

    stbrp_pack_rects(context, rects, range_count);

    for (uint32_t i = 0; i < range_count; i++) {
      font.info.chars[i].rect.from.x = rects[i].x + extra_padding;
      font.info.chars[i].rect.from.y = rects[i].y + extra_padding;
      
      if (rects[i].was_packed) {
        // Copy bitmap to atlas
        for (uint32_t y = 0; y < font.info.chars[i].rect.size.y; y++) {
          for (uint32_t x = 0; x < font.info.chars[i].rect.size.x; x++) {
            uint32_t i_dst = (rects[i].y + extra_padding + y) * font.atlas_bitmap_width + (rects[i].x + extra_padding + x);
            i_dst *= channels;

            uint32_t i_src = (y * font.info.chars[i].rect.size.x + x);
            
            ((char*) font.atlas_bitmap)[i_dst + 0] = 0xff;
            ((char*) font.atlas_bitmap)[i_dst + 1] = 0xff;
            ((char*) font.atlas_bitmap)[i_dst + 2] = 0xff;
            ((char*) font.atlas_bitmap)[i_dst + 3] = fontchar_data[i][i_src];
          }
        } 

      } else {
        kb_log_warn("Char {} not packed!", i);
      }
      
      // UV Rect
      font.info.chars[i].uv_rect_from.x  = (font.info.chars[i].rect.from.x + font_padding) / (float) font.atlas_bitmap_width;
      font.info.chars[i].uv_rect_from.y  = (font.info.chars[i].rect.from.y + font_padding) / (float) font.atlas_bitmap_height;

      font.info.chars[i].uv_rect_to.x    = (font.info.chars[i].rect.from.x + font.info.chars[i].rect.size.x - + font_padding) / (float) font.atlas_bitmap_width;
      font.info.chars[i].uv_rect_to.y    = (font.info.chars[i].rect.from.y + font.info.chars[i].rect.size.y - + font_padding) / (float) font.atlas_bitmap_height;
    }

    // Write
    kb_font_data_write(&font, rwops_out);
    
    KB_DEFAULT_FREE(context);
    KB_DEFAULT_FREE(nodes);
    KB_DEFAULT_FREE(rects);
    KB_DEFAULT_FREE(fontchar_data);
    KB_DEFAULT_FREE(font_data);
  }

  exit_val = EXIT_SUCCESS;
end:
  kb_rwops_close(rwops_in);
  kb_rwops_close(rwops_out);

  return exit_val;
}
