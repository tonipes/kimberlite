// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/foundation.h>
#include <kb/graphics.h>

#ifdef __cplusplus
extern "C" {
#endif

KB_API void kb_texture_read   (kb_texture_data* dst, kb_stream* src);
KB_API void kb_texture_write  (const kb_texture_data* src, kb_stream* dst);

#ifdef __cplusplus
}
#endif
