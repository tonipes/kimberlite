#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define KB_RESOURCE_HASHED_FUNC_DECLS(name, handle_t, create_info_t)                              \
  void      kb_##name##_set_name(handle_t handle, const char* name);                              \
  void      kb_##name##_set_hash(handle_t handle, Hash hash);                                     \
  void      kb_##name##_remove_name(handle_t handle);                                             \
  bool      kb_##name##_has(const char* name);                                                    \
  handle_t  kb_##name##_get(const char* name);                                                    \
  handle_t  kb_##name##_get_or_allocate(const char* name);                                        \
  
#define KB_RESOURCE_CORE_FUNC_DECLS(name, handle_t, create_info_t)                                \
  handle_t kb_##name##_allocate();                                                                \
  handle_t kb_##name##_create(const create_info_t info);                                          \
  void     kb_##name##_destroy(handle_t handle);                                                  \
  uint32_t kb_##name##_count();                                                                   \

#ifdef __cplusplus
}
#endif

// c++ implementation
#ifdef __cplusplus

#include <kb/freelist.h>
#include <kb/table.h>
#include <kb/crt.h>
#include <kb/log.h>

template <typename Handle, typename CreateInfo, typename Ref, uint16_t Capacity>
struct ResourceData {
  ResourceData() {
    kb_table_create     (&table, Capacity);
    kb_freelist_create  (&freelist, Capacity);
    kb_memset(&refcount, 0, sizeof(refcount));
  }

  ~ResourceData() {
    kb_table_destroy     (&table);
    kb_freelist_destroy  (&freelist);
  }
  
  void purge() {
    uint32_t* dense = kb_freelist_get_dense(&freelist);
    for (uint32_t i = 0; i < freelist.count; ++i) {
      kb_log_debug("Purge: ({}) {}", i, dense[i]);
    }
  }

  Ref       refs      [Capacity];
  uint16_t  refcount  [Capacity];
  Freelist  freelist;
  Table     table;
};

#define KB_RESOURCE_DATA_CORE_DEF(t_name, handle_t, create_info_t, ref_t, capacity)               \
  ResourceData<handle_t, create_info_t, ref_t, capacity> t_name##_data;                           \
  void      kb_##t_name##_construct  (handle_t h, const create_info_t& info);                     \
  void      kb_##t_name##_destruct   (handle_t h);                                                \
  void      kb_##t_name##_remove_name(handle_t handle) {                                          \
    kb_table_remove(&(t_name##_data.table), handle.idx);                                          \
  }                                                                                               \
  inline auto& t_name##_ref(handle_t handle) {                                                    \
    return t_name##_data.refs[handle.idx];                                                        \
  }                                                                                               \
  handle_t  kb_##t_name##_allocate() { return {                                                   \
    kb_freelist_take(&(t_name##_data.freelist)) };                                                \
  }                                                                                               \
  void      kb_##t_name##_free(handle_t handle) {                                                 \
    kb_freelist_return(&(t_name##_data.freelist), handle.idx);                                    \
  }                                                                                               \
  void kb_##t_name##_destroy(handle_t handle) {                                                   \
    kb_##t_name##_free(handle);                                                                   \
    kb_##t_name##_remove_name(handle);                                                            \
    kb_##t_name##_destruct(handle);                                                               \
    t_name##_ref(handle) = {};                                                                    \
  }                                                                                               \
  handle_t kb_##t_name##_create(const create_info_t info) {                                       \
    handle_t handle = kb_##t_name##_allocate();                                                   \
    t_name##_ref(handle) = {};                                                                    \
    kb_##t_name##_construct(handle, info);                                                        \
    return handle;                                                                                \
  }                                                                                               \
  uint32_t kb_##t_name##_count() {                                                                \
    return kb_freelist_count(&(t_name##_data.freelist));                                          \
  }                                                                                               \
  void kb_##t_name##_purge() {                                                                    \
    uint32_t* dense = kb_freelist_get_dense(&t_name##_data.freelist);                             \
    uint32_t count = kb_##t_name##_count();                                                       \
    for (uint32_t i = 0; i < count; ++i) {                                                        \
      kb_##t_name##_destroy({dense[0]});                                                          \
    }                                                                                             \
  }   

#define KB_RESOURCE_DATA_HASHED_DEF(t_name, handle_t)                                             \
  void kb_##t_name##_set_hash(handle_t handle, Hash _hash) {                                      \
    kb_table_insert(&(t_name##_data.table), _hash, handle.idx);                                   \
  }                                                                                               \
  void kb_##t_name##_set_name(handle_t handle, const char* name) {                                \
    kb_##t_name##_set_hash(handle, kb_hash_string(name));                                         \
  }                                                                                               \
  bool kb_##t_name##_has(const char* name) {                                                      \
    return kb_table_get(&(t_name##_data.table), kb_hash_string(name)) != InvalidHandle;           \
  }                                                                                               \
  handle_t kb_##t_name##_get(const char* name) {                                                  \
    return { (uint16_t) kb_table_get(&(t_name##_data.table), kb_hash_string(name)) };             \
  }                                                                                               \
  handle_t kb_##t_name##_get_or_allocate(const char* name) {                                      \
    if (kb_##t_name##_has(name)) return kb_##t_name##_get(name);                                  \
    handle_t handle = kb_##t_name##_allocate();                                                   \
    kb_##t_name##_set_name(handle, name);                                                         \
    return handle;                                                                                \
  }                                                                                               \

#endif
