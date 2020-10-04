#pragma once

#include <kb/core.h>

#ifdef __cplusplus
extern "C" {
#endif  

typedef struct {
  uint32_t  elem_size;
  uint32_t  capacity;
  uint32_t  count;
  void*     data;
} kb_array;

KB_API void     kb_array_create     (kb_array* array, uint32_t elem_size, uint32_t capacity);
KB_API void     kb_array_destroy    (kb_array* array);
KB_API void     kb_array_reset      (kb_array* array);
KB_API void     kb_array_copy       (kb_array* dst, const kb_array* src);
KB_API void*    kb_array_begin      (kb_array* array);
KB_API void*    kb_array_back       (kb_array* array);
KB_API void*    kb_array_end        (kb_array* array);
KB_API uint32_t kb_array_count      (kb_array* array);
KB_API uint32_t kb_array_capacity   (kb_array* array);
KB_API void     kb_array_reserve    (kb_array* array, uint32_t size);
KB_API void*    kb_array_get        (kb_array* array, uint32_t index);
KB_API void     kb_array_set        (kb_array* array, uint32_t index);
KB_API void     kb_array_push_back  (kb_array* array, void* data);
KB_API void     kb_array_pop_back   (kb_array* array);

#ifdef __cplusplus
}
#endif

// c++ implementation
#ifdef __cplusplus
// KB_API_INLINE void kb_create(kb_array& array, uint32_t elem_size, uint32_t capacity) {
//   kb_array_create(&array, elem_size, capacity);
// }

// KB_API_INLINE void kb_destroy(kb_array& array) {
//   kb_array_destroy(&array);
// }

// KB_API_INLINE void kb_reset(kb_array& array) {
//   kb_array_reset(&array);
// }

// KB_API_INLINE void kb_copy(kb_array& dst, const kb_array& src) {
//   kb_array_copy(&dst, &src);
// }

#endif
