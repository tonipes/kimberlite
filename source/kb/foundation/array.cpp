// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/foundation/array.h>
#include <kb/foundation/alloc.h>
#include <kb/foundation/crt.h>

static bool array_need_to_grow(kb_array* array, uint64_t n) {
  KB_ASSERT_NOT_NULL(array);

  return array->cap <= array->pos + n;
}

static void array_maybe_grow(kb_array* array, uint64_t n) {
  KB_ASSERT_NOT_NULL(array);

  if (!array_need_to_grow(array, n)) return;

  uint64_t dbl_cap  = 2 * array->cap;
  uint64_t needed   = array->pos + n;
  uint64_t new_cap  = dbl_cap > needed ? dbl_cap : needed;

  kb_array_reserve(array, new_cap);
}

KB_API void kb_array_create(kb_array* array, uint64_t elem_size, uint64_t capacity) {
  KB_ASSERT_NOT_NULL(array);

  array->data       = KB_DEFAULT_ALLOC(elem_size * capacity);
  array->cap        = capacity;
  array->elem_size  = elem_size;

  kb_array_reset(array);
}

KB_API void kb_array_destroy(kb_array* array) {
  KB_ASSERT_NOT_NULL(array);

  if (array->cap == 0) return;

  KB_DEFAULT_FREE(array->data);
  kb_memset(array, '\0', sizeof(kb_array));
}

KB_API void kb_array_reset(kb_array* array) {
  KB_ASSERT_NOT_NULL(array);

  array->pos = 0;
  kb_memset(array->data, 0, array->cap * array->elem_size);
}

KB_API void kb_array_copy(kb_array* dst, const kb_array* src) {
  KB_ASSERT_NOT_NULL(dst);
  KB_ASSERT_NOT_NULL(src);

  kb_array_create(dst, src->elem_size, src->cap);
  
  kb_memcpy(dst->data, src->data, src->elem_size * src->cap);

  dst->pos = src->pos;
}

KB_API uint64_t kb_array_count(const kb_array* array) {
  KB_ASSERT_NOT_NULL(array);

  return array->pos;
}

KB_API uint64_t kb_array_capacity(const kb_array* array) {
  KB_ASSERT_NOT_NULL(array);

  return array->cap;
}

KB_API void kb_array_reserve(kb_array* array, uint64_t cap) {
  KB_ASSERT_NOT_NULL(array);

  if (array->cap >= cap) return;

  array->data = KB_DEFAULT_REALLOC(array->data, array->elem_size * cap);
  
  KB_ASSERT(array->data, "Failed to reallocate array");

  array->cap = cap;
}

KB_API void kb_array_resize(kb_array* array, uint64_t size) {
  kb_array_reserve(array, size);
  array->pos = size;
}

KB_API void* kb_array_at(const kb_array* array, uint64_t index) {
  KB_ASSERT_NOT_NULL(array);

  if (index > array->cap) return nullptr;
  
  return &(((uint8_t*) array->data)[array->elem_size * index]);
}

KB_API void kb_array_pop_back(kb_array* array) {
  KB_ASSERT_NOT_NULL(array);

  array->pos--;
}

KB_API void kb_array_push_back(kb_array* array, void* data) {
  KB_ASSERT_NOT_NULL(array);

  array_maybe_grow(array, 1);
  kb_memcpy(kb_array_end(array), data, array->elem_size);
  array->pos++;
}

KB_API void* kb_array_begin(kb_array* array) {
  KB_ASSERT_NOT_NULL(array);

  return kb_array_at(array, 0);
}

KB_API void* kb_array_back(kb_array* array) {
  KB_ASSERT_NOT_NULL(array);

  return kb_array_at(array, array->pos > 0 ? array->pos - 1 : 0);
}

KB_API void* kb_array_end(kb_array* array) {
  KB_ASSERT_NOT_NULL(array);

  return kb_array_at(array, array->pos);
}
