#pragma once

#include <kb/core.h>

#ifdef __cplusplus
extern "C" {
#endif

KB_HANDLE(FontHandle);

typedef struct {
  uint32_t  codepoint;
  int       advance;
  int       left_bearing;
  IntRect   rect;
  Int2      bbox_from;
  Int2      bbox_to;
  Float2    uv_rect_from;
  Float2    uv_rect_to;
} FontChar;

typedef struct {
  int   ascent;
  int   descent;
  int   line_gap;
  float scale_factor;
  float pixel_height;
} FontBaseInfo;

typedef struct {
  FontBaseInfo base_info;
  uint32_t  char_count;
  FontChar* chars;

  uint32_t  atlas_bitmap_width;
  uint32_t  atlas_bitmap_height;
  uint32_t  atlas_bitmap_size;
  
  void*     atlas_bitmap;
} Font;

KB_API void         kb_font_read      (Font* font, RWops* rwops);
KB_API void         kb_font_write     (const Font* font, RWops* rwops);
KB_API void         kb_font_dump_info (const Font* font);
KB_API FontHandle   kb_font_load      (RWops* rwops);

#ifdef __cplusplus
}
#endif
