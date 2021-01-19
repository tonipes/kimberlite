// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kbextra/texture.h>

void kb_texture_read(kb_texture_data* dst, kb_stream* src) {
  kb_read(src, dst->header);
  dst->data = KB_DEFAULT_ALLOC(dst->data_size);
  kb_stream_read(src, dst->data, 1, dst->data_size);
}

void kb_texture_write(const kb_texture_data* src, kb_stream* dst) {  
  kb_write(dst, src->header);
  kb_stream_write(dst, src->data, 1, src->data_size);
}
