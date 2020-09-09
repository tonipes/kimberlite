#pragma once

#define KB_RESOURCE_NON_HASHED_FUNC_DEFS(name, handle_t)                                          \
  void kb_##name##_remove_hash(handle_t handle) { }

#define KB_RESOURCE_HASHED_FUNC_DEFS(name, handle_t, create_info_t, _table)                       \
  void kb_##name##_set_hash(Hash key, handle_t handle) {                                          \
    _table.insert(key, handle.idx);                                                               \
  }                                                                                               \
  void kb_##name##_remove_hash(handle_t handle) {                                                 \
    _table.remove(handle.idx);                                                                    \
  }                                                                                               \
  bool kb_##name##_has(Hash key) {                                                                \
    return _table.find(key) == InvalidHandle;                                                     \
  }                                                                                               \
  handle_t kb_##name##_get(Hash key) {                                                            \
    handle_t handle = {_table.find(key)};                                                         \
    return handle;                                                                                \
  }                                                                                               \
  handle_t kb_##name##_get_or_allocate(Hash key) {                                                \
    if (_table.has(key)) return kb_##name##_get(key);                                             \
    handle_t handle = kb_##name##_allocate();                                                     \
    kb_##name##_set_hash(key, handle);                                                            \
    return handle;                                                                                \
  }                                                                                               \
  handle_t kb_##name##_create_named(Hash key, const create_info_t info) {                         \
    handle_t handle = kb_##name##_create(info);                                                   \
    kb_##name##_set_hash(key, handle);                                                            \
    return handle;                                                                                \
  }                                                                                               \

#define KB_RESOURCE_CORE_FUNC_DEFS(name, handle_t, create_info_t, _container, _alloc)             \
  void kb_##name##_construct(handle_t h, const create_info_t& info);                              \
  void kb_##name##_destruct(handle_t h);                                                          \
  handle_t kb_##name##_allocate() {                                                               \
    return { _alloc.alloc() };                                                                    \
  }                                                                                               \
  void kb_##name##_free(handle_t handle) {                                                        \
    _alloc.free(handle.idx);                                                                      \
  }                                                                                               \
  void kb_##name##_destroy(handle_t handle) {                                                     \
    kb_##name##_free(handle);                                                                     \
    kb_##name##_remove_hash(handle);                                                              \
    kb_##name##_destruct(handle);                                                                 \
    _container[handle.idx] = {};                                                                  \
  }                                                                                               \
  handle_t kb_##name##_create(const create_info_t info) {                                         \
    handle_t handle = kb_##name##_allocate();                                                     \
    _container[handle.idx] = {};                                                                  \
    kb_##name##_construct(handle, info);                                                          \
    return handle;                                                                                \
  }                                                                                               \
  uint32_t kb_##name##_count() {                                                                  \
    return _alloc.count();                                                                        \
  }                                                                                               \
  inline auto& name##_ref(handle_t handle) {                                                      \
    return _container[handle.idx];                                                                \
  }                                                                                               \
