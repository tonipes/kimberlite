// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#define KB_TOOL_ONLY

#include <kb/log.h>

#include <kb/foundation/core.h>
#include <kb/foundation/time.h>

#include <kbextra/cliargs.h>
#include <kbextra/font.h>

#include "kb/foundation.cpp"
#include "kb/log.cpp"

#include "kbextra/cliargs.cpp"
#include "kbextra/font.cpp"

#include "platform/platform_rwops_stdio.cpp"

#define STB_RECT_PACK_IMPLEMENTATION 
#include "stb/stb_rect_pack.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION 
#include "stb/stb_truetype.h"

#define EXIT_FAIL     1
#define EXIT_SUCCESS  0

#define MAX_RANGES 8

const uint32_t  font_padding          = 16;
const uint32_t  font_on_edge_value    = 128;
const float     font_pixel_dist_scale = 8.0f;
const float     font_size             = 64.0f;
const uint32_t  channels              = 4;

void write_func(void* user, void* data, int size) {
  kb_stream* io = (kb_stream*) user;
  kb_stream_write(io, data, 1, size);
}

void print_help(const char* error = nullptr) {
  if (error != nullptr) printf("%s\n", error);

  printf(
    "Kimberlite fontc\n"
    "\tUsage: fontc --input <in> --output <out> --from <range start> --to <range end>\n"
  );
}

struct Range {
  int32_t from;
  int32_t to;
};

int main(int argc, const char* argv[]) {
  int exit_val = EXIT_FAIL;
  
  kb_stream*       rwops_in            = nullptr;
  kb_stream*       rwops_out           = nullptr;
  const char*     in_filepath         = nullptr;
  const char*     out_filepath        = nullptr;
  int32_t         range_count         = 0;
  Range           ranges[MAX_RANGES]  = {0};

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

  rwops_in = kb_stream_open_file(in_filepath, KB_FILE_MODE_READ);
  if (!rwops_in) {
    print_help("Unable to open input file");
    goto end;
  }

  rwops_out = kb_stream_open_file(out_filepath, KB_FILE_MODE_WRITE);
  if (!rwops_out) {
    print_help("Unable to open output file");
    goto end;
  }

  range_count = KB_MIN(kb_cliargs_count(&args, "from"), kb_cliargs_count(&args, "to"));
  range_count = KB_MIN(MAX_RANGES, range_count);
  
  for (uint32_t i = 0; i < range_count; ++i) {
    const char* from_str  = kb_cliargs_find(&args, "from",  i);
    const char* to_str    = kb_cliargs_find(&args, "to",  i);
    
    if (from_str == NULL || to_str == NULL) break;

    kb_fromstr_int(&ranges[i].from, from_str);
    kb_fromstr_int(&ranges[i].to, to_str);
  }

  for (uint32_t i = 0; i < range_count; ++i) {
    kb_log_debug("Range with {} chars: ({} - {})", ranges[i].to - ranges[i].from, ranges[i].from, ranges[i].to);
  }

  // Load (modified from https://github.com/raysan5/raylib/blob/master/src/text.c#L568)
  {
    uint32_t total_char_count = 0;
    for (uint32_t i = 0; i < range_count; ++i) {
      total_char_count += ranges[i].to - ranges[i].from;
    }

    unsigned char** fontchar_data = KB_DEFAULT_ALLOC_TYPE(unsigned char*, total_char_count);

    uint64_t font_data_size = kb_stream_size(rwops_in);
    unsigned char* font_data = (unsigned char*) KB_DEFAULT_ALLOC(font_data_size);
    uint64_t read = kb_stream_read(rwops_in, font_data, 1, font_data_size);
    kb_log_debug("Size: {}, Read: {}", font_data_size, read);

    stbtt_fontinfo font_info;
    stbtt_InitFont(&font_info, font_data, stbtt_GetFontOffsetForIndex(font_data, 0));
    
    float scale_factor = stbtt_ScaleForPixelHeight(&font_info, font_size);
    
    font.info.pixel_height  = font_size;
    font.info.scale_factor  = scale_factor;
    font.info.char_count    = total_char_count;
    font.info.padding       = font_padding;
    
    font.info.chars = KB_DEFAULT_ALLOC_TYPE(kb_font_char, font.info.char_count);

    stbtt_GetFontVMetrics(&font_info, &font.info.ascent, &font.info.descent, &font.info.line_gap);
    
    uint32_t current_char_slot = 0;

    for (uint32_t range_i = 0; range_i < range_count; ++range_i) {
      uint32_t range_char_count = ranges[range_i].to - ranges[range_i].from;

      for (uint32_t char_i = 0; char_i < range_char_count; char_i++) {
        uint32_t char_slot = current_char_slot++;

        kb_font_char& font_char = font.info.chars[char_slot];

        int codepoint = ranges[range_i].from + char_i;

        font_char.codepoint = codepoint;
        
        fontchar_data[char_slot] = stbtt_GetCodepointSDF(
          &font_info, scale_factor, codepoint, 
          font_padding, font_on_edge_value, font_pixel_dist_scale, 
          &font_char.rect.size.x, &font_char.rect.size.y, 
          &font_char.rect.from.x, &font_char.rect.from.y
        );

        stbtt_GetCodepointHMetrics(&font_info, codepoint, &font_char.advance, &font_char.left_bearing);
          
        stbtt_GetCodepointBitmapBox(&font_info, codepoint, scale_factor, scale_factor, 
          &font_char.bbox_from.x, &font_char.bbox_from.y, 
          &font_char.bbox_to.x, &font_char.bbox_to.y
        );
        
        font_char.advance *= scale_factor;

        // font_char.bbox_from.x -= font_padding;
        // font_char.bbox_from.y -= font_padding;
        // font_char.bbox_to.x   += font_padding;
        // font_char.bbox_to.y   += font_padding;
      }
    }

    // Pack to atlas
    float required_area = 0;

    for (uint32_t i = 0; i < font.info.char_count; i++) {
      if (fontchar_data[i] == NULL) continue;
      required_area += ((font.info.chars[i].rect.size.x) * (font.info.chars[i].rect.size.y));
    }

    float guess_size = sqrtf(required_area);
    int image_size = (int) powf(2, ceilf(logf((float)guess_size) / logf(2)));

    font.atlas_bitmap_width  = image_size;
    font.atlas_bitmap_height = image_size;
    
    font.atlas_bitmap_size = font.atlas_bitmap_width * font.atlas_bitmap_height * channels;
    font.atlas_bitmap = KB_DEFAULT_ALLOC(font.atlas_bitmap_size);
    kb_memset(font.atlas_bitmap, 0, font.atlas_bitmap_size);

    stbrp_context*  context  = KB_DEFAULT_ALLOC_TYPE(stbrp_context, 1);
    stbrp_node*     nodes    = KB_DEFAULT_ALLOC_TYPE(stbrp_node, font.info.char_count);
    stbrp_rect*     rects    = KB_DEFAULT_ALLOC_TYPE(stbrp_rect, font.info.char_count);

    kb_log_info("Atlas size {} x {}", font.atlas_bitmap_width, font.atlas_bitmap_height);
    stbrp_init_target(context, font.atlas_bitmap_width, font.atlas_bitmap_height, nodes, font.info.char_count);
    
    for (uint32_t i = 0; i < font.info.char_count; i++) {
      rects[i].id = i;
      rects[i].w = font.info.chars[i].rect.size.x;
      rects[i].h = font.info.chars[i].rect.size.y;
    }

    stbrp_pack_rects(context, rects, font.info.char_count);

    for (uint32_t i = 0; i < font.info.char_count; i++) {
      font.info.chars[i].rect.from.x = rects[i].x;
      font.info.chars[i].rect.from.y = rects[i].y;
      
      if (rects[i].was_packed) {
        // Copy bitmap to atlas
        for (uint32_t y = 0; y < font.info.chars[i].rect.size.y; y++) {
          for (uint32_t x = 0; x < font.info.chars[i].rect.size.x; x++) {
            uint32_t i_dst = (rects[i].y + y) * font.atlas_bitmap_width + (rects[i].x + x);
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
      // font.info.chars[i].uv_rect_from.x  = (font.info.chars[i].rect.from.x + font_padding) / (float) font.atlas_bitmap_width;
      // font.info.chars[i].uv_rect_from.y  = (font.info.chars[i].rect.from.y + font_padding) / (float) font.atlas_bitmap_height;

      // font.info.chars[i].uv_rect_to.x    = (font.info.chars[i].rect.from.x + font.info.chars[i].rect.size.x - font_padding) / (float) font.atlas_bitmap_width;
      // font.info.chars[i].uv_rect_to.y    = (font.info.chars[i].rect.from.y + font.info.chars[i].rect.size.y - font_padding) / (float) font.atlas_bitmap_height;

      font.info.chars[i].uv_rect_from.x  = (font.info.chars[i].rect.from.x) / (float) font.atlas_bitmap_width;
      font.info.chars[i].uv_rect_from.y  = (font.info.chars[i].rect.from.y) / (float) font.atlas_bitmap_height;

      font.info.chars[i].uv_rect_to.x    = (font.info.chars[i].rect.from.x + font.info.chars[i].rect.size.x) / (float) font.atlas_bitmap_width;
      font.info.chars[i].uv_rect_to.y    = (font.info.chars[i].rect.from.y + font.info.chars[i].rect.size.y) / (float) font.atlas_bitmap_height;
    }
    
    // Write
    kb_font_data_write(&font, rwops_out);
    
    // Debug output
    stbi_write_bmp("fontc_output.png", font.atlas_bitmap_width, font.atlas_bitmap_height, 4, font.atlas_bitmap);
    
    KB_DEFAULT_FREE(context);
    KB_DEFAULT_FREE(nodes);
    KB_DEFAULT_FREE(rects);
    KB_DEFAULT_FREE(fontchar_data);
    KB_DEFAULT_FREE(font_data);
  }

  exit_val = EXIT_SUCCESS;
end:
  kb_stream_close(rwops_in);
  kb_stream_close(rwops_out);

  return exit_val;
}
