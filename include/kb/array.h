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

typedef struct kb_array {
  uint64_t  elem_size;
  uint64_t  capacity;
  uint64_t  count;
  void*     data;
} kb_array;

KB_API void     kb_array_create     (kb_array* array, uint64_t elem_size, uint64_t capacity);
KB_API void     kb_array_destroy    (kb_array* array);
KB_API void     kb_array_reset      (kb_array* array);
KB_API void     kb_array_copy       (kb_array* dst, const kb_array* src);
KB_API void*    kb_array_begin      (kb_array* array);
KB_API void*    kb_array_back       (kb_array* array);
KB_API void*    kb_array_end        (kb_array* array);
KB_API uint64_t kb_array_count      (kb_array* array);
KB_API uint64_t kb_array_capacity   (kb_array* array);
KB_API void     kb_array_reserve    (kb_array* array, uint64_t size);
KB_API void     kb_array_resize     (kb_array* array, uint64_t size);
KB_API void*    kb_array_get        (kb_array* array, uint64_t index);
KB_API void     kb_array_set        (kb_array* array, uint64_t index);
KB_API void     kb_array_push_back  (kb_array* array, void* data);
KB_API void     kb_array_pop_back   (kb_array* array);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#endif
