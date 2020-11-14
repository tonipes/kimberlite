// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/texture.h>

#include <kb/crt.h>
#include <kb/alloc.h>
#include <kb/rwops.h>
#include <kb/renderer.h>

void kb_texture_read(kb_texture_data* dst, kb_rwops* src) {
  kb_read(src, dst->header);
  dst->data = KB_DEFAULT_ALLOC(dst->data_size);
  kb_rwops_read(src, dst->data, dst->data_size);
}

void kb_texture_write(const kb_texture_data* src, kb_rwops* dst) {  
  kb_write(dst, src->header);
  kb_rwops_write(dst, src->data, src->data_size);
}
