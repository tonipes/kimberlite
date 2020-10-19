// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/core.h>

#ifdef __cplusplus
extern "C" {
#endif

KB_API int64_t  kb_time_get_raw();
KB_API int64_t  kb_time_get_current();
KB_API int64_t  kb_time_get_frequency();
KB_API float    kb_time();

#ifdef __cplusplus
}
#endif
