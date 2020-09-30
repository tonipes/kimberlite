#pragma once

#include <kb/api.h>
#include <kb/types.h>

#ifdef __cplusplus
extern "C" {
#endif  

typedef struct {
  uint32_t  elem_size;
  uint32_t  capacity;
  uint32_t  count;
  void*     data;
} Array;

KB_API void     kb_array_create     (Array* array, uint32_t elem_size, uint32_t capacity);
KB_API void     kb_array_destroy    (Array* array);
KB_API void     kb_array_reset      (Array* array);
KB_API void     kb_array_copy       (Array* dst, const Array* src);

KB_API void*    kb_array_begin      (Array* array);
KB_API void*    kb_array_back       (Array* array);
KB_API void*    kb_array_end        (Array* array);
// KB_API void*    kb_array_next       (Array* array, void* current);

KB_API uint32_t kb_array_count      (Array* array);
KB_API uint32_t kb_array_capacity   (Array* array);
KB_API void     kb_array_reserve    (Array* array, uint32_t size);

KB_API void*    kb_array_get        (Array* array, uint32_t index);
KB_API void     kb_array_set        (Array* array, uint32_t index);

KB_API void     kb_array_push_back  (Array* array, void* data);
KB_API void     kb_array_pop_back   (Array* array);


#ifdef __cplusplus
}
#endif

// c++ implementation
#ifdef __cplusplus
// KB_API_INLINE void kb_create(Array& array, uint32_t elem_size, uint32_t capacity) {
//   kb_array_create(&array, elem_size, capacity);
// }

// KB_API_INLINE void kb_destroy(Array& array) {
//   kb_array_destroy(&array);
// }

// KB_API_INLINE void kb_reset(Array& array) {
//   kb_array_reset(&array);
// }

// KB_API_INLINE void kb_copy(Array& dst, const Array& src) {
//   kb_array_copy(&dst, &src);
// }

#endif
