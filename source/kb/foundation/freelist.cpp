// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/foundation/freelist.h>
#include <kb/foundation/alloc.h>
#include <kb/foundation/crt.h>

KB_API void kb_freelist_create(kb_freelist* freelist, uint32_t cap) {
  KB_ASSERT_NOT_NULL(freelist);

  freelist->data = KB_DEFAULT_ALLOC_TYPE(uint32_t, 2 * cap);
  freelist->cap = cap;
  kb_freelist_reset(freelist);
}

KB_API void kb_freelist_destroy(kb_freelist* freelist) {
  KB_ASSERT_NOT_NULL(freelist);

  KB_DEFAULT_FREE(freelist->data);
  kb_memset(freelist, 0, sizeof(kb_freelist));
}

KB_API void kb_freelist_reset(kb_freelist* freelist) {
  KB_ASSERT_NOT_NULL(freelist);

  freelist->pos = 0;
  
  uint32_t* dense = kb_freelist_get_dense(freelist);
  for (uint32_t i = 0, num = freelist->cap; i < num; ++i) {
    dense[i] = i;
  }
  
  uint32_t* sparse = kb_freelist_get_sparse(freelist);
  for (uint32_t i = 0, num = freelist->cap; i < num; ++i) {
    sparse[i] = UINT32_MAX;
  }
}

KB_API void kb_freelist_copy(kb_freelist* dst, const kb_freelist* src) {
  KB_ASSERT_NOT_NULL(dst);
  KB_ASSERT_NOT_NULL(src);
  
  kb_freelist_create(dst, src->cap);
  
  kb_memcpy(dst->data, src->data, 2 * src->cap);
  
  dst->pos = src->pos;
}

KB_API uint32_t kb_freelist_take(kb_freelist* freelist) {
  KB_ASSERT_NOT_NULL(freelist);

  if (freelist->pos < freelist->cap) {
    uint32_t index = freelist->pos;
    freelist->pos++;

    uint32_t* dense  = kb_freelist_get_dense(freelist);
    uint32_t* sparse = kb_freelist_get_sparse(freelist);

    uint32_t handle = dense[index];
    sparse[handle]  = index;
    
    return handle;
  }

  return UINT32_MAX;
}

KB_API bool kb_freelist_free(kb_freelist* freelist, uint32_t handle) {
  KB_ASSERT_NOT_NULL(freelist);

  if (freelist->pos == 0 || handle > freelist->cap)
    return false;

  uint32_t* dense  = kb_freelist_get_dense(freelist);
  uint32_t* sparse = kb_freelist_get_sparse(freelist);

  uint32_t index = sparse[handle];
  if (index == UINT32_MAX) 
    return false;

  freelist->pos--;

  uint32_t last = dense[freelist->pos];
  
  dense   [freelist->pos] = handle;
  sparse  [last]          = index;
  dense   [index]         = last;
  
  sparse  [handle]        = UINT32_MAX;
  
  return true;
}

KB_API uint32_t kb_freelist_count(const kb_freelist* freelist) {
  KB_ASSERT_NOT_NULL(freelist);

  return freelist->pos;
}

KB_API uint32_t kb_freelist_capacity(const kb_freelist* freelist) {
  KB_ASSERT_NOT_NULL(freelist);

  return freelist->cap;
}

KB_API uint32_t* kb_freelist_get_sparse(const kb_freelist* freelist) {
  KB_ASSERT_NOT_NULL(freelist);

  return &kb_freelist_get_dense(freelist)[freelist->cap];
}

KB_API uint32_t* kb_freelist_get_dense(const kb_freelist* freelist) {
  KB_ASSERT_NOT_NULL(freelist);

  return freelist->data;
}
