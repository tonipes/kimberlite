// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/freelist.h>

#include <kb/alloc.h>
#include <kb/crt.h>

KB_API void kb_freelist_create(kb_freelist* freelist, uint32_t capacity) {
  KB_ASSERT_NOT_NULL(freelist);

  freelist->handles = KB_DEFAULT_ALLOC_TYPE(uint32_t, 2 * capacity);
  freelist->capacity = capacity;
  kb_freelist_reset(freelist);
}

KB_API void kb_freelist_destroy(kb_freelist* freelist) {
  KB_ASSERT_NOT_NULL(freelist);

  KB_DEFAULT_FREE(freelist->handles);
  kb_memset(freelist, 0, sizeof(kb_freelist));
}

KB_API void kb_freelist_reset(kb_freelist* freelist) {
  KB_ASSERT_NOT_NULL(freelist);

  freelist->count = 0;
  
  uint32_t* dense = kb_freelist_get_dense(freelist);
  for (uint32_t i = 0, num = freelist->capacity; i < num; ++i) {
    dense[i] = i;
  }
  
  uint32_t* sparse = kb_freelist_get_sparse(freelist);
  for (uint32_t i = 0, num = freelist->capacity; i < num; ++i) {
    sparse[i] = UINT32_MAX;
  }
}

KB_API uint32_t kb_freelist_take(kb_freelist* freelist) {
  KB_ASSERT_NOT_NULL(freelist);

  if (freelist->count < freelist->capacity) {
    uint32_t index = freelist->count;
    freelist->count++;

    uint32_t* dense  = kb_freelist_get_dense(freelist);
    uint32_t* sparse = kb_freelist_get_sparse(freelist);

    uint32_t handle = dense[index];
    sparse[handle]  = index;
    
    return handle;
  }

  return UINT32_MAX;
}

KB_API bool kb_freelist_return(kb_freelist* freelist, uint32_t handle) {
  KB_ASSERT_NOT_NULL(freelist);

  if (freelist->count == 0 || handle > freelist->capacity) 
    return false;

  uint32_t* dense  = kb_freelist_get_dense(freelist);
  uint32_t* sparse = kb_freelist_get_sparse(freelist);

  uint32_t index = sparse[handle];
  if (index == UINT32_MAX) 
    return false;

  freelist->count--;

  uint32_t last = dense[freelist->count];
  
  dense   [freelist->count] = handle;
  sparse  [last]            = index;
  dense   [index]           = last;
  
  sparse  [handle]          = UINT32_MAX;
  
  return true;
}

KB_API uint32_t kb_freelist_count(const kb_freelist* freelist) {
  KB_ASSERT_NOT_NULL(freelist);

  return freelist->count;
}

KB_API uint32_t* kb_freelist_get_sparse(kb_freelist* freelist) {
  KB_ASSERT_NOT_NULL(freelist);

  return &kb_freelist_get_dense(freelist)[freelist->capacity];
}

KB_API uint32_t* kb_freelist_get_dense(kb_freelist* freelist) {
  KB_ASSERT_NOT_NULL(freelist);

  return freelist->handles;
}
