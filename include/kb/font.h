// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/core.h>
#include <kb/handle.h>
#include <kb/math.h>
#include <kb/rwops.h>
#include <kb/table.h>
#include <kb/resource.h>
#include <kb/renderer.h>

KB_HANDLE(kb_font);

#ifdef __cplusplus
extern "C" {
#endif

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
  kb_pipeline   pipeline;
  kb_texture    atlas;
  uint32_t      atlas_height;
  uint32_t      atlas_width;
} kb_font_create_info;

KB_RESOURCE_HASHED_FUNC_DECLS (font, kb_font, kb_font_create_info);
KB_RESOURCE_ALLOC_FUNC_DECLS  (font, kb_font, kb_font_create_info);

KB_API void   kb_font_data_read                     (kb_font_data* font, kb_rwops* rwops);
KB_API void   kb_font_data_write                    (const kb_font_data* font, kb_rwops* rwops);
KB_API void   kb_font_data_dump_info                (const kb_font_data* font);
KB_API void   kb_font_data_destroy                  (kb_font_data* font);


KB_API void   kb_font_break_line                    (kb_font_info* info, float* current_y);
KB_API void   kb_font_quad_advance                  (kb_font_info* info, int codepoint, float* current_x, float* current_y, FloatRect* pos, FloatRect* uv); 
KB_API Real32 kb_font_get_line_height               (kb_font_info* info);
KB_API Real32 kb_font_get_string_height             (kb_font_info* info, const char* str);
KB_API Real32 kb_font_get_string_width              (kb_font_info* info, const char* str);
KB_API Real32 kb_font_get_string_line_width         (kb_font_info* info, const char* str);

KB_API void   kb_encoder_bind_font                  (kb_encoder encoder, kb_font font);
KB_API void   kb_encoder_submit_text                (kb_encoder encoder, kb_font font, const char* str, Float2 pos, float font_height, uint32_t instance_count);

KB_API void   kb_font_load                          (kb_font target, kb_pipeline pipeline, kb_rwops* rwops);

#ifdef __cplusplus
}
#endif
