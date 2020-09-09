#pragma once

#include <kb/hash.hpp>

#define KB_RESOURCE_NON_HASHED_FUNC_DEFS(t_name, handle_t)                                        \
  void kb_##t_name##_remove_name(handle_t handle) { }

#define KB_RESOURCE_HASHED_FUNC_DEFS(t_name, handle_t, create_info_t, _table)                     \
  void kb_##t_name##_set_name(handle_t handle, const char* name) {                                \
    _table.insert(kb_hash_string(name), handle.idx);                                              \
  }                                                                                               \
  void kb_##t_name##_remove_name(handle_t handle) {                                               \
    _table.remove(handle.idx);                                                                    \
  }                                                                                               \
  bool kb_##t_name##_has(const char* name) {                                                      \
    return _table.find(kb_hash_string(name)) == InvalidHandle;                                    \
  }                                                                                               \
  handle_t kb_##t_name##_get(const char* name) {                                                  \
    handle_t handle = {_table.find(kb_hash_string(name))};                                        \
    return handle;                                                                                \
  }                                                                                               \
  handle_t kb_##t_name##_get_or_allocate(const char* name) {                                      \
    if (_table.has(kb_hash_string(name))) return kb_##t_name##_get(name);                         \
    handle_t handle = kb_##t_name##_allocate();                                                   \
    kb_##t_name##_set_name(handle, name);                                                         \
    return handle;                                                                                \
  }                                                                                               \

#define KB_RESOURCE_CORE_FUNC_DEFS(t_name, handle_t, create_info_t, _container, _alloc)           \
  void kb_##t_name##_construct(handle_t h, const create_info_t& info);                            \
  void kb_##t_name##_destruct(handle_t h);                                                        \
  handle_t kb_##t_name##_allocate() {                                                             \
    return { _alloc.alloc() };                                                                    \
  }                                                                                               \
  void kb_##t_name##_free(handle_t handle) {                                                      \
    _alloc.free(handle.idx);                                                                      \
  }                                                                                               \
  void kb_##t_name##_destroy(handle_t handle) {                                                   \
    kb_##t_name##_free(handle);                                                                   \
    kb_##t_name##_remove_name(handle);                                                            \
    kb_##t_name##_destruct(handle);                                                               \
    _container[handle.idx] = {};                                                                  \
  }                                                                                               \
  handle_t kb_##t_name##_create(const create_info_t info) {                                       \
    handle_t handle = kb_##t_name##_allocate();                                                   \
    _container[handle.idx] = {};                                                                  \
    kb_##t_name##_construct(handle, info);                                                        \
    return handle;                                                                                \
  }                                                                                               \
  uint32_t kb_##t_name##_count() {                                                                \
    return _alloc.count();                                                                        \
  }                                                                                               \
  inline auto& t_name##_ref(handle_t handle) {                                                    \
    return _container[handle.idx];                                                                \
  }                                                                                               \
