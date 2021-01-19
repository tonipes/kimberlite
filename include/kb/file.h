// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include "foundation/stream.h"

#ifdef __cplusplus
extern "C" {
#endif

KB_API kb_stream*  kb_stream_open_file  (const char* path, kb_file_mode mode);

#ifdef __cplusplus
}
#endif
