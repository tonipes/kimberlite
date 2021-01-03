// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/core.h>
#include <kb/handle.h>
#include <kb/math.h>
#include <kb/stream.h>
#include <kb/table.h>
#include <kb/resource.h>
#include <kb/graphics.h>

typedef uint32_t kb_wchar;

#define KB_UNICODE_CODEPOINT_MAX 0xFFFF
#define KB_UNICODE_CODEPOINT_INVALID 0xFFFD

#ifdef __cplusplus
extern "C" {
#endif

KB_HANDLE(kb_font);

typedef struct kb_font_char {
  uint32_t      codepoint;
  int           advance;
  int           left_bearing;
  IntRect       rect;
  Int2          bbox_from;
  Int2          bbox_to;
  Float2        uv_rect_from;
  Float2        uv_rect_to;
} kb_font_char;

typedef struct kb_font_info {
  int32_t       ascent;
  int32_t       descent;
  int32_t       line_gap;
  float         scale_factor;
  float         pixel_height;
  float         padding;
  uint32_t      char_count;
  kb_font_char* chars;
  kb_table      char_table;
} kb_font_info;

typedef struct kb_font_data {
  kb_font_info  info;
  uint32_t      atlas_bitmap_width;
  uint32_t      atlas_bitmap_height;
  uint32_t      atlas_bitmap_size;
  void*         atlas_bitmap;
} kb_font_data;

typedef struct kb_font_render_info {
  Float4        color;
  float         outline;
  float         smoothing;    
} kb_font_render_info;

typedef struct kb_font_create_info {
  kb_font_info  info;
} kb_font_create_info;

KB_RESOURCE_HASHED_FUNC_DECLS (font, kb_font, kb_font_create_info);
KB_RESOURCE_ALLOC_FUNC_DECLS  (font, kb_font, kb_font_create_info);

// Convert UTF8 to kb_wchar. Return bytecount
KB_API uint32_t kb_from_utf8  (kb_wchar* out_char, const char* in, uint32_t count);

// Convert kb_wchar to UTF8. Return bytecount
KB_API uint32_t kb_to_utf8    (char* dst, int dst_size, kb_wchar* c);

// Count codepoints in string
KB_API uint32_t kb_count_utf8 (const char* str, uint32_t n);

KB_API void   kb_font_data_read               (kb_font_data* font, kb_stream* rwops);
KB_API void   kb_font_data_write              (const kb_font_data* font, kb_stream* rwops);
KB_API void   kb_font_data_dump_info          (const kb_font_data* font);
KB_API void   kb_font_data_destroy            (kb_font_data* font);

KB_API void   kb_font_break_line              (kb_font_info* info, float* current_y);
KB_API void   kb_font_quad_advance            (kb_font_info* info, int codepoint, float* current_x, float* current_y, FloatRect* pos, FloatRect* uv); 

KB_API float  kb_font_get_line_height         (kb_font_info* info);
KB_API Float2 kb_font_get_string_dimensions   (kb_font_info* info, const char* str, uint32_t len);

KB_API void   kb_encoder_submit_text          (kb_encoder encoder, kb_font font, const char* str, uint32_t len, Float2 origin, Float2 scale, Float2 align, Float2 offset, uint32_t instance_count);

KB_API kb_font_info* kb_font_get_info         (kb_font font);

  
#ifdef __cplusplus
}
#endif
