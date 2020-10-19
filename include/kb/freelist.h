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

typedef struct kb_freelist {
  uint32_t*   handles;
  uint32_t    count;
  uint32_t    capacity;
} kb_freelist;

KB_API void       kb_freelist_create        (kb_freelist* freelist, uint32_t capacity);
KB_API void       kb_freelist_destroy       (kb_freelist* freelist);
KB_API void       kb_freelist_reset         (kb_freelist* freelist);
KB_API uint32_t   kb_freelist_take          (kb_freelist* freelist);
KB_API bool       kb_freelist_return        (kb_freelist* freelist, uint32_t handle);
KB_API uint32_t*  kb_freelist_get_sparse    (kb_freelist* freelist);
KB_API uint32_t*  kb_freelist_get_dense     (kb_freelist* freelist);
KB_API uint32_t   kb_freelist_count         (const kb_freelist* freelist);

#ifdef __cplusplus
}
#endif
