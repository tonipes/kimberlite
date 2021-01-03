// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kbextra/font.h>

#include <kb/core.h>
#include <kb/stream.h>
#include <kb/alloc.h>
#include <kb/graphics.h>
#include <kb/stream.h>
#include <kb/log.h>

#include <kbextra/vertex.h>

KB_API uint32_t kb_count_utf8(const char* str, uint32_t len) {
  const char* in_text     = str;
  const char* in_text_end = in_text + len;

  int char_count = 0;

  while ((!in_text_end || in_text < in_text_end) && *in_text) {
    kb_wchar c;
    in_text += kb_from_utf8(&c, in_text, in_text_end - in_text);
    if (c == 0) break;
    char_count++;
  }
  return char_count;
}

// From ImGui
KB_API uint32_t kb_to_utf8(char* buf, int buf_size, kb_wchar c) {
  if (c < 0x80) {
    buf[0] = (char)c;
    return 1;
  }

  if (c < 0x800) {
    if (buf_size < 2) return 0;
    buf[0] = (char)(0xc0 + (c >> 6));
    buf[1] = (char)(0x80 + (c & 0x3f));
    return 2;
  }

  if (c < 0x10000) {
    if (buf_size < 3) return 0;
    buf[0] = (char)(0xe0 + (c >> 12));
    buf[1] = (char)(0x80 + ((c>> 6) & 0x3f));
    buf[2] = (char)(0x80 + ((c ) & 0x3f));
    return 3;
  }

  if (c <= 0x10FFFF) {
    if (buf_size < 4) return 0;
    buf[0] = (char)(0xf0 + (c >> 18));
    buf[1] = (char)(0x80 + ((c >> 12) & 0x3f));
    buf[2] = (char)(0x80 + ((c >> 6) & 0x3f));
    buf[3] = (char)(0x80 + ((c ) & 0x3f));
    return 4;
  }

  // Invalid code point, the max unicode is 0x10FFFF
  return 0;
}

// From ImGui
KB_API uint32_t kb_from_utf8(kb_wchar* dst, const char* src, uint32_t len) {
  const char* in_text     = src;
  const char* in_text_end = src + len;

  unsigned int c = (unsigned int)-1;
  const unsigned char* str = (const unsigned char*)in_text;
  if (!(*str & 0x80)) {
    c = (unsigned int)(*str++);
    *dst = c;
    return 1;
  }
  if ((*str & 0xe0) == 0xc0) {
    *dst = KB_UNICODE_CODEPOINT_INVALID; // will be invalid but not end of string
    if (in_text_end && in_text_end - (const char*)str < 2) return 1;
    if (*str < 0xc2) return 2;
    c = (unsigned int)((*str++ & 0x1f) << 6);
    if ((*str & 0xc0) != 0x80) return 2;
    c += (*str++ & 0x3f);
    *dst = c;
    return 2;
  }
  if ((*str & 0xf0) == 0xe0) {
    *dst = KB_UNICODE_CODEPOINT_INVALID; // will be invalid but not end of string
    if (in_text_end && in_text_end - (const char*)str < 3) return 1;
    if (*str == 0xe0 && (str[1] < 0xa0 || str[1] > 0xbf)) return 3;
    if (*str == 0xed && str[1] > 0x9f) return 3; // str[1] < 0x80 is checked below
    c = (unsigned int)((*str++ & 0x0f) << 12);
    if ((*str & 0xc0) != 0x80) return 3;
    c += (unsigned int)((*str++ & 0x3f) << 6);
    if ((*str & 0xc0) != 0x80) return 3;
    c += (*str++ & 0x3f);
    *dst = c;
    return 3;
  }
  if ((*str & 0xf8) == 0xf0) {
    *dst = KB_UNICODE_CODEPOINT_INVALID; // will be invalid but not end of string
    if (in_text_end && in_text_end - (const char*)str < 4) return 1;
    if (*str > 0xf4) return 4;
    if (*str == 0xf0 && (str[1] < 0x90 || str[1] > 0xbf)) return 4;
    if (*str == 0xf4 && str[1] > 0x8f) return 4; // str[1] < 0x80 is checked below
    c = (unsigned int)((*str++ & 0x07) << 18);
    if ((*str & 0xc0) != 0x80) return 4;
    c += (unsigned int)((*str++ & 0x3f) << 12);
    if ((*str & 0xc0) != 0x80) return 4;
    c += (unsigned int)((*str++ & 0x3f) << 6);
    if ((*str & 0xc0) != 0x80) return 4;
    c += (*str++ & 0x3f);
    // utf-8 encodings of values used in surrogate pairs are invalid
    if ((c & 0xFFFFF800) == 0xD800) return 4;
    // If codepoint does not fit in ImWchar, use replacement character U+FFFD instead
    if (c > KB_UNICODE_CODEPOINT_MAX) c = KB_UNICODE_CODEPOINT_INVALID;
    *dst = c;
    return 4;
  }
  
  *dst = 0;
  
  return 0;
}

KB_API void kb_font_data_read(kb_font_data* font, kb_stream* rwops) {
  KB_ASSERT_NOT_NULL(font);
  KB_ASSERT_NOT_NULL(rwops);

  if (!kb_stream_check_magic(rwops, KB_CONFIG_FILE_MAGIC_FONT)) {
    kb_printf("Did not find correct magic number!\n");
    return;
  }

  kb_read(rwops, font->info.ascent);
  kb_read(rwops, font->info.descent);
  kb_read(rwops, font->info.line_gap);
  kb_read(rwops, font->info.scale_factor);
  kb_read(rwops, font->info.pixel_height);
  kb_read(rwops, font->info.padding);
  kb_read(rwops, font->info.char_count);
  
  font->info.chars = KB_DEFAULT_ALLOC_TYPE(kb_font_char, font->info.char_count);
  for (uint32_t i = 0; i < font->info.char_count; i++) {
    kb_read(rwops, font->info.chars[i]);
  }

  kb_read(rwops, font->atlas_bitmap_width);
  kb_read(rwops, font->atlas_bitmap_height);
  kb_read(rwops, font->atlas_bitmap_size);

  font->atlas_bitmap = KB_DEFAULT_ALLOC(font->atlas_bitmap_size);
  kb_stream_read(rwops, font->atlas_bitmap, 1,  font->atlas_bitmap_size);

  // Build char table
  kb_table_create(&font->info.char_table, font->info.char_count);
  
  for (uint32_t i = 0; i < font->info.char_count; i++) {
    kb_table_insert(&font->info.char_table, font->info.chars[i].codepoint, i);
  }
}

KB_API void kb_font_data_write(const kb_font_data* font, kb_stream* rwops) {
  KB_ASSERT_NOT_NULL(font);
  KB_ASSERT_NOT_NULL(rwops);

  kb_write(rwops, KB_CONFIG_FILE_MAGIC_FONT);

  // kb_write(rwops, font->info);
  kb_write(rwops, font->info.ascent);
  kb_write(rwops, font->info.descent);
  kb_write(rwops, font->info.line_gap);
  kb_write(rwops, font->info.scale_factor);
  kb_write(rwops, font->info.pixel_height);
  kb_write(rwops, font->info.padding);
  kb_write(rwops, font->info.char_count);

  for (uint32_t i = 0; i < font->info.char_count; i++) {
    kb_write(rwops, font->info.chars[i]);
  }

  kb_write(rwops, font->atlas_bitmap_width);
  kb_write(rwops, font->atlas_bitmap_height);
  kb_write(rwops, font->atlas_bitmap_size);

  kb_stream_write(rwops, font->atlas_bitmap, 1, font->atlas_bitmap_size);
}

KB_API void kb_font_data_dump_info(const kb_font_data* font) {
  kb_printf("Font: \n");

  kb_printf("\tChars: %u\n", font->info.char_count);
  
  for (uint32_t i = 0; i < font->info.char_count; ++i) {
    kb_font_char* fc = &font->info.chars[i];

    kb_printf("\t\tChar (%c) adv %i, UV [(%f %f) - (%f %f)]\n",
      fc->codepoint, fc->advance,
      font->info.chars[i].uv_rect_from.x,  font->info.chars[i].uv_rect_from.y,
      font->info.chars[i].uv_rect_to.x,    font->info.chars[i].uv_rect_to.y
    );
    kb_printf("\t\t\tfrom: (%i, %i), to: (%i, %i) lsb %i\n",
      fc->bbox_from.x,  fc->bbox_from.y, 
      fc->bbox_to.x,    fc->bbox_to.y,
      fc->left_bearing
    );
  }

  kb_printf("\tAtlas: (%u %u) of size %u\n", font->atlas_bitmap_width, font->atlas_bitmap_height, font->atlas_bitmap_size);
}

KB_API void kb_font_data_destroy(kb_font_data* font) {
  KB_ASSERT_NOT_NULL(font);

  KB_DEFAULT_FREE(font->atlas_bitmap);
  KB_DEFAULT_FREE(font->info.chars);
  kb_table_destroy(&font->info.char_table);
}

void kb_font_break_line(kb_font_info* info, float* current_y) {
  KB_ASSERT_NOT_NULL(info);
  KB_ASSERT_NOT_NULL(current_y);

  *current_y += kb_font_get_line_height(info);
}

void kb_font_quad_advance(kb_font_info* info, int codepoint, float* current_x, float* current_y, FloatRect* pos, FloatRect* uv) {  
  KB_ASSERT_NOT_NULL(info);
  KB_ASSERT_NOT_NULL(current_x);
  KB_ASSERT_NOT_NULL(current_y);

  uint32_t char_idx = kb_table_get(&info->char_table, codepoint);

  if (char_idx == UINT32_MAX) { return; }

  const kb_font_char* fc = &info->chars[char_idx];
  
  if (pos != NULL) {
    pos->from.x  = *current_x + fc->bbox_from.x;
    pos->to.x    = *current_x + fc->bbox_to.x;
    pos->from.y  = *current_y + fc->bbox_from.y;
    pos->to.y    = *current_y + fc->bbox_to.y;
  }

  if (uv != NULL) {
    uv->from.x  = fc->uv_rect_from.x;
    uv->to.x    = fc->uv_rect_to.x;
    uv->from.y  = fc->uv_rect_from.y;
    uv->to.y    = fc->uv_rect_to.y;
  }

  *current_x += fc->advance;
}

KB_API float kb_font_get_line_height(kb_font_info* info) {
  return (info->ascent - info->descent + info->line_gap) * info->scale_factor;
}

KB_API uint32_t kb_font_advance_line(kb_font_info* info, const char* str, uint32_t len, float* width) {    
  Float2 current_pos { 0.0f, 0.0f };
  
  FloatRect pos;

  float pmin = FLOATMAX;
  float pmax = FLOATMIN;
  
  uint32_t cpos = 0;
  uint32_t codeps = 0;

  while (cpos < len) {
    kb_wchar codep;
    cpos += kb_from_utf8(&codep, &str[cpos], len - cpos);

    if (codep == '\n' || codep == '\0') { break; }

    kb_font_quad_advance(info, codep, &current_pos.x, &current_pos.y, &pos, nullptr);

    pmin = min(pmin, pos.from.x);
    pmax = max(pmax, pos.to.x);
    codeps++;
  }
  
  if (width != NULL) {
    *width = codeps > 0 ? (pmax - pmin) * (1.0f / info->pixel_height) : 0.0f;
  }
  
  return cpos;
}

KB_API Float2 kb_font_get_string_dimensions(kb_font_info* info, const char* str, uint32_t len) {
  uint32_t pos = 0;
  
  uint32_t lines = 0;
  
  float width = 0.0f;

  while (pos < len) {
    float cwidth = 0.0f;
    uint32_t c = kb_font_advance_line(info, &str[pos], len - pos, &cwidth);
    
    if (c == 0) break;

    pos += c;
    width = max(width, cwidth);
    lines++;
  };
  
  return {
    width,
    (float) lines,
  };
}

// #undef KB_TOOL_ONLY

#ifndef KB_TOOL_ONLY

struct kb_font_ref {
  kb_font_info  info;
};

KB_RESOURCE_STORAGE_DEF       (font, kb_font, kb_font_ref, KB_CONFIG_MAX_FONTS);
KB_RESOURCE_ALLOC_FUNC_DEF    (font, kb_font, kb_font_create_info, KB_CONFIG_MAX_FONTS);
KB_RESOURCE_DATA_HASHED_DEF   (font, kb_font);

void kb_font_construct(kb_font handle, const kb_font_create_info info) {
  font_ref(handle)->info            = info.info;
  font_ref(handle)->info.chars      = (kb_font_char*) kb_memdup(info.info.chars, sizeof(kb_font_char) * info.info.char_count);
  kb_table_copy(&font_ref(handle)->info.char_table, &info.info.char_table);
}

void kb_font_destruct(kb_font handle) { 
  KB_DEFAULT_FREE(font_ref(handle)->info.chars);
  kb_table_destroy(&font_ref(handle)->info.char_table);
}

void kb_encoder_submit_text(kb_encoder encoder, kb_font font, const char* str, uint32_t len, Float2 origin, Float2 scale, Float2 align, Float2 offset, uint32_t instance_count) {
  uint32_t codepoints = kb_count_utf8(str, len);

  if (codepoints == 0) return;

  kb_font_ref* fnt = font_ref(font);

  uint32_t max_vertices = codepoints * 4;
  uint32_t max_indices  = codepoints * 6;
  
  uint32_t vertex_alloc_size = max_vertices  * sizeof(kb_simple_vertex);
  uint32_t index_alloc_size  = max_indices   * sizeof(uint16_t);

  kb_simple_vertex* vertex_data_buffer = (kb_simple_vertex*)  kb_graphics_transient_alloc(vertex_alloc_size,  256);
  uint16_t*         index_data_buffer  = (uint16_t*)          kb_graphics_transient_alloc(index_alloc_size,   256);

  kb_simple_vertex* vertex_data = vertex_data_buffer;
  uint16_t*         index_data  = index_data_buffer;
  
  uint32_t indices  = 0;
  uint32_t vertices = 0;

  Float2 scale_factor = scale * (1.0f /  fnt->info.pixel_height);
  scale_factor.y *= -1;

  float pos_offset = (float) (fnt->info.ascent) * fnt->info.scale_factor;
  
  Float2 str_dim = kb_font_get_string_dimensions(&fnt->info, str, len);

  float origin_x = pos_offset * offset.x;
  float origin_y = pos_offset * offset.y;

  Float2 current_pos {
    origin_x,
    pos_offset + origin_y
  };

  FloatRect pos;
  FloatRect uv;
  
  uint32_t strpos = 0;
  
  Float2 padding = { fnt->info.padding, fnt->info.padding };

  float line_width = 0.0f;
  kb_font_advance_line(&fnt->info, str, len, &line_width);
  
  Float2 align_offset = {
    -line_width * fnt->info.pixel_height,
    -str_dim.y * fnt->info.pixel_height,
  };

  for (uint32_t i = 0; i < codepoints; ++i) {
    kb_wchar codep;
    strpos += kb_from_utf8(&codep, &str[strpos], len - strpos);

    if (codep == '\n') { // Linebreak
      kb_font_break_line(&fnt->info, &current_pos.y);
      kb_font_advance_line(&fnt->info, &str[strpos], len - strpos, &line_width);
      align_offset.x = -line_width * fnt->info.pixel_height;
      current_pos.x = origin_x;
      continue;
    }

    if (codep == '\t') { // Tab
      kb_font_quad_advance(&fnt->info, codep, &current_pos.x, &current_pos.y, &pos, &uv);
      continue;
    }
    kb_font_quad_advance(&fnt->info, codep, &current_pos.x, &current_pos.y, &pos, &uv);
    
    Float2 current_align = (align_offset * align);
  
    Float2 from = scale_factor * (pos.from - padding + current_align);
    Float2 to   = scale_factor * (pos.to + padding + current_align);
    
    kb_simple_vertex verts[4] = {
      { { origin.x + from.x,  origin.y - from.y,   0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { uv.from.x , uv.from.y } },
      { { origin.x + from.x,  origin.y - to.y,     0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { uv.from.x , uv.to.y   } },
      { { origin.x + to.x,    origin.y - to.y,     0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { uv.to.x ,   uv.to.y   } },
      { { origin.x + to.x,    origin.y - from.y,   0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { uv.to.x ,   uv.from.y } },
    };
    
    kb_memcpy(vertex_data, verts, sizeof(verts));
    
    index_data[0] = uint16_t(vertices + 0);
    index_data[1] = uint16_t(vertices + 1);
    index_data[2] = uint16_t(vertices + 2);
    index_data[3] = uint16_t(vertices + 0);
    index_data[4] = uint16_t(vertices + 2);
    index_data[5] = uint16_t(vertices + 3);

    vertex_data += 4;
    vertices    += 4;

    index_data  += 6;
    indices     += 6;
  }

  uint32_t vertex_data_buffer_offset  = kb_graphics_transient_offset(vertex_data_buffer);
  uint32_t index_data_buffer_offset   = kb_graphics_transient_offset(index_data_buffer);
  
  kb_encoder_push(encoder);
  
  kb_encoder_bind_buffer(encoder, 0, {}, vertex_data_buffer_offset);
  kb_encoder_bind_index_buffer(encoder, {}, index_data_buffer_offset, KB_INDEX_TYPE_16); 
  kb_encoder_submit(encoder, 0, 0, indices, instance_count);

  kb_encoder_pop(encoder);
}

KB_API kb_font_info* kb_font_get_info(kb_font font) {
  return &font_ref(font)->info;
}

#endif
