// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t kb_handle_idx;

static inline bool kb_is_valid_idx(kb_handle_idx idx) { return idx != UINT32_MAX && idx != 0; }  \

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
template <typename T>
static inline bool kb_is_valid(T handle) { return kb_is_valid_idx(handle.idx); }

template <typename T>
static inline kb_handle_idx kb_to_arr(T handle) { return handle.idx - 1; }
#endif

#define KB_HANDLE(name_t) typedef struct name_t { kb_handle_idx idx; } name_t


#define KB_ASSERT_VALID(handle) assert(kb_is_valid_idx(handle.idx) && "Invalid handle: '" #handle "'")
#define KB_HANDLE_FROM_ARRAY(idx) { idx + 1 }
#define KB_HANDLE_TO_ARRAY(handle) handle.idx - 1
