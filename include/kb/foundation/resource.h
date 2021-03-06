// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include "freelist.h"
#include "table.h"
#include "crt.h"

#ifdef __cplusplus
extern "C" {
#endif

#define KB_RESOURCE_HASHED_FUNC_DECLS(t_name, handle_t, create_info_t)         \
  void      kb_##t_name##_mark(handle_t handle, kb_hash hash);                 \
  void      kb_##t_name##_unmark(handle_t handle);                             \
  bool      kb_##t_name##_has(kb_hash hash);                                   \
  handle_t  kb_##t_name##_get(kb_hash hash);                                   \
  handle_t  kb_##t_name##_get_existing(kb_hash hash);

#define KB_RESOURCE_ALLOC_FUNC_DECLS(t_name, handle_t, create_info_t)          \
  handle_t kb_##t_name##_allocate(void);                                       \
  handle_t kb_##t_name##_create(const create_info_t info);                     \
  void     kb_##t_name##_destroy(handle_t handle);                             \
  uint32_t kb_##t_name##_count(void);                                          \
  void     kb_##t_name##_purge(void);                                          \
  void     kb_##t_name##_construct(handle_t h, const create_info_t info);      \
  void     kb_##t_name##_destruct(handle_t h);                                 \
  bool     kb_##t_name##_is_initialized(handle_t h);
  
#define KB_RESOURCE_STORAGE_DEF(t_name, handle_t, ref_t, cap)                  \
  ref_t t_name##_refs [cap];                                                   \
  ref_t* t_name##_ref(handle_t handle) {                                       \
    return &t_name##_refs[handle.idx + 1];                                     \
  }
  
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

template <typename kb_handle, typename create_info_t, uint16_t cap>
struct kb_resource_slot_allocator {
  kb_resource_slot_allocator() {
    kb_table_create     (&table, cap);
    kb_freelist_create  (&freelist, cap);
  }

  ~kb_resource_slot_allocator() {
    kb_table_destroy     (&table);
    kb_freelist_destroy  (&freelist);
  }

  kb_freelist   freelist;
  kb_table      table;
  bool          initialized[cap] = { false };
};


#define KB_RESOURCE_ALLOC_FUNC_DEF(t_name, handle_t, create_info_t, cap)          \
  kb_resource_slot_allocator<handle_t, create_info_t, cap> t_name##_data;         \
  void kb_##t_name##_unmark(handle_t handle) {                                    \
    kb_table_remove(&(t_name##_data.table), kb_to_arr(handle));                   \
  }                                                                               \
  handle_t kb_##t_name##_allocate() {                                             \
    return (handle_t){ kb_freelist_take(&(t_name##_data.freelist)) + 1 };         \
  }                                                                               \
  void kb_##t_name##_free(handle_t handle) {                                      \
    kb_freelist_free(&(t_name##_data.freelist), kb_to_arr(handle));               \
  }                                                                               \
  void kb_##t_name##_destroy(handle_t handle) {                                   \
    kb_##t_name##_free(handle);                                                   \
    kb_##t_name##_unmark(handle);                                                 \
    kb_##t_name##_destruct(handle);                                               \
  }                                                                               \
  handle_t kb_##t_name##_create(const create_info_t info) {                       \
    handle_t handle = kb_##t_name##_allocate();                                   \
    kb_##t_name##_construct(handle, info);                                        \
    return handle;                                                                \
  }                                                                               \
  uint32_t kb_##t_name##_count() {                                                \
    return kb_freelist_count(&(t_name##_data.freelist));                          \
  }                                                                               \
  void kb_##t_name##_purge() {                                                    \
    uint32_t* dense = kb_freelist_get_dense(&t_name##_data.freelist);             \
    uint32_t count = kb_##t_name##_count();                                       \
    for (uint32_t i = 0; i < count; ++i) {                                        \
      kb_##t_name##_destroy(KB_HANDLE_FROM_ARRAY(dense[0]));                      \
    }                                                                             \
  }   

#define KB_RESOURCE_DATA_HASHED_DEF(t_name, handle_t)                             \
  void kb_##t_name##_mark(handle_t handle, kb_hash _hash) {                       \
    kb_table_insert(&(t_name##_data.table), _hash, kb_to_arr(handle));            \
  }                                                                               \
  bool kb_##t_name##_has(kb_hash _hash) {                                         \
    return kb_is_valid_idx(kb_##t_name##_get_existing(_hash).idx);                \
  }                                                                               \
  handle_t kb_##t_name##_get_existing(kb_hash _hash) {                            \
    return KB_HANDLE_FROM_ARRAY(kb_table_get(&(t_name##_data.table), _hash));     \
  }                                                                               \
  handle_t kb_##t_name##_get(kb_hash hash) {                                      \
    if (kb_##t_name##_has(hash)) return kb_##t_name##_get_existing(hash);         \
    handle_t handle = kb_##t_name##_allocate();                                   \
    kb_##t_name##_mark(handle, hash);                                             \
    return handle;                                                                \
  }                                                                               \
  bool kb_##t_name##_is_initialized(handle_t handle) {                            \
    return t_name##_data.initialized[kb_to_arr(handle)];                          \
  }                                                                               \
  void kb_##t_name##_set_initialized(handle_t handle, bool value) {               \
    t_name##_data.initialized[kb_to_arr(handle)] = value;                         \
  }

#endif
