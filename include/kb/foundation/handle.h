// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t kb_handle_idx;

KB_API_INLINE bool kb_is_valid_idx(kb_handle_idx idx) { return idx != UINT32_MAX && idx != 0; }

#define KB_HANDLE(name_t) typedef struct name_t { kb_handle_idx idx; } name_t

#ifdef __cplusplus
}
#endif

#define KB_ASSERT_VALID(handle) assert(kb_is_valid_idx(handle.idx) && "Invalid handle: '" #handle "'")
#define KB_IS_VALID(handle) kb_is_valid_idx(handle.idx)

#define KB_HANDLE_FROM_ARRAY(idx) { idx + 1 }
#define KB_HANDLE_TO_ARRAY(handle) handle.idx - 1

#ifdef __cplusplus
template <typename T>
KB_API_INLINE bool kb_is_valid(T handle) { return kb_is_valid_idx(handle.idx); }

template <typename T>
KB_API_INLINE kb_handle_idx kb_to_arr(T handle) { return handle.idx - 1; }
#endif
