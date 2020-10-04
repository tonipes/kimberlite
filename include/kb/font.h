#pragma once

#include <kb/core.h>
#include <kb/handle.h>
#include <kb/math.h>
#include <kb/rwops.h>

KB_HANDLE(kb_font);

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint32_t  codepoint;
  int       advance;
  int       left_bearing;
  IntRect   rect;
  Int2      bbox_from;
  Int2      bbox_to;
  Float2    uv_rect_from;
  Float2    uv_rect_to;
} kb_font_char;

typedef struct {
  int   ascent;
  int   descent;
  int   line_gap;
  float scale_factor;
  float pixel_height;
} kb_font_info;

typedef struct {
  kb_font_info  info;
  uint32_t      char_count;
  kb_font_char* chars;
  uint32_t      atlas_bitmap_width;
  uint32_t      atlas_bitmap_height;
  uint32_t      atlas_bitmap_size;
  void*         atlas_bitmap;
} kb_font_data;

KB_API void kb_font_read      (kb_font_data* font, kb_rwops* rwops);
KB_API void kb_font_write     (const kb_font_data* font, kb_rwops* rwops);
KB_API void kb_font_dump_info (const kb_font_data* font);

#ifdef __cplusplus
}
#endif
