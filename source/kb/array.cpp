// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/array.h>

#include <kb/alloc.h>
#include <kb/crt.h>

static bool array_need_to_grow(kb_array* array, uint64_t n) {
  KB_ASSERT_NOT_NULL(array);

  return array->capacity <= array->count + n;
}

static void array_maybe_grow(kb_array* array, uint64_t n) {
  KB_ASSERT_NOT_NULL(array);

  if (!array_need_to_grow(array, n)) return;

  uint64_t dbl_cap  = 2 * array->capacity;
  uint64_t needed   = array->count + n;
  uint64_t new_cap  = dbl_cap > needed ? dbl_cap : needed;

  kb_array_reserve(array, new_cap);
}

KB_API void kb_array_create(kb_array* array, uint64_t elem_size, uint64_t capacity) {
  KB_ASSERT_NOT_NULL(array);

  array->data       = KB_DEFAULT_ALLOC(elem_size * capacity);
  array->capacity   = capacity;
  array->elem_size  = elem_size;

  kb_array_reset(array);
}

KB_API void kb_array_destroy(kb_array* array) {
  KB_ASSERT_NOT_NULL(array);

  if (array->capacity == 0) return;

  KB_DEFAULT_FREE(array->data);
  kb_memset(array, '\0', sizeof(kb_array));
}

KB_API void kb_array_reset(kb_array* array) {
  KB_ASSERT_NOT_NULL(array);

  array->count = 0;
}

KB_API void kb_array_copy(kb_array* dst, const kb_array* src) {
  KB_ASSERT_NOT_NULL(dst);
  KB_ASSERT_NOT_NULL(src);

  kb_array_create(dst, src->elem_size, src->capacity);
  
  kb_memcpy(dst->data, src->data, src->elem_size * src->capacity);

  dst->count = src->count;
}

KB_API uint64_t kb_array_count(kb_array* array) {
  KB_ASSERT_NOT_NULL(array);

  return array->count;
}

KB_API uint64_t kb_array_capacity(kb_array* array) {
  KB_ASSERT_NOT_NULL(array);

  return array->capacity;
}

KB_API void kb_array_reserve(kb_array* array, uint64_t cap) {
  KB_ASSERT_NOT_NULL(array);

  if (array->capacity >= cap) return;

  array->data = KB_DEFAULT_REALLOC(array->data, array->elem_size * cap);
  
  KB_ASSERT(array->data, "Failed to reallocate array");

  array->capacity = cap;
}

KB_API void kb_array_resize(kb_array* array, uint64_t size) {
  kb_array_reserve(array, size);
  array->count = size;
}

KB_API void* kb_array_get(kb_array* array, uint64_t index) {
  KB_ASSERT_NOT_NULL(array);

  if (index > array->capacity) return nullptr;
  return &(((uint8_t*) array->data)[array->elem_size * index]);
}

KB_API void kb_array_pop_back(kb_array* array) {
  KB_ASSERT_NOT_NULL(array);

  array->count--;
}

KB_API void kb_array_push_back(kb_array* array, void* data) {
  KB_ASSERT_NOT_NULL(array);

  array_maybe_grow(array, 1);
  kb_memcpy(kb_array_end(array), data, array->elem_size);
  array->count++;
}

KB_API void* kb_array_begin(kb_array* array) {
  KB_ASSERT_NOT_NULL(array);

  return kb_array_get(array, 0);
}

KB_API void* kb_array_back(kb_array* array) {
  KB_ASSERT_NOT_NULL(array);

  return kb_array_get(array, array->count > 0 ? array->count - 1 : 0);
}

KB_API void* kb_array_end(kb_array* array) {
  KB_ASSERT_NOT_NULL(array);

  return kb_array_get(array, array->count);
}
