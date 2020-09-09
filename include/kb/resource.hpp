#pragma once

// #define KB_RESOURCE_DEFAULT_CONSTRUCT_FUNC(name, handle, create_info, data_container)             \
//   void construct_##name(handle h, const create_info info) { data_container[h.idx] = {}; } 

// #define KB_RESOURCE_DEFAULT_DESTRUCT_FUNC(name, handle, data_container)                           \
//   void destruct_##name(handle h) { data_container[h.idx] = {}; } 

#define KB_RESOURCE_NON_HASHED_FUNC_DEFS(name, handle_t)                                          \
  void remove_##name##_hash(handle_t handle) { }

#define KB_RESOURCE_HASHED_FUNC_DEFS(name, handle_t, create_info_t, _table)                       \
  void set_##name##_hash(Hash key, handle_t handle) {                                             \
    _table.insert(key, handle.idx);                                                               \
  }                                                                                               \
  void remove_##name##_hash(handle_t handle) {                                                    \
    _table.remove(handle.idx);                                                                    \
  }                                                                                               \
  bool has_##name (Hash key) {                                                                    \
    return _table.find(key) == InvalidHandle;                                                     \
  }                                                                                               \
  handle_t get_##name(Hash key) {                                                                 \
    handle_t handle = {_table.find(key)};                                                         \
    return handle;                                                                                \
  }                                                                                               \
  handle_t get_or_allocate_##name(Hash key) {                                                     \
    if (_table.has(key)) return get_##name(key);                                                  \
    handle_t handle = allocate_##name();                                                          \
    set_##name##_hash(key, handle);                                                               \
    return handle;                                                                                \
  }                                                                                               \
  handle_t create_named_##name(Hash key, const create_info_t info) {                              \
    auto handle = create_##name(info);                                                            \
    set_##name##_hash(key, handle);                                                               \
    return handle;                                                                                \
  }                                                                                               \

#define KB_RESOURCE_CORE_FUNC_DEFS(name, handle_t, create_info_t, _container, _alloc)             \
  void construct_##name(handle_t h, const create_info_t& info);                                   \
  void destruct_##name(handle_t h);                                                               \
  handle_t allocate_##name() { return {_alloc.alloc()}; }                                         \
  void free_##name(handle_t handle) {                                                             \
    _alloc.free(handle.idx);                                                                      \
  }                                                                                               \
  void destroy_##name(handle_t handle) {                                                          \
    free_##name(handle);                                                                          \
    remove_##name##_hash(handle);                                                                 \
    destruct_##name(handle);                                                                      \
    _container[handle.idx] = {};                                                                  \
  }                                                                                               \
  handle_t create_##name(const create_info_t info) {                                              \
    auto handle = allocate_##name();                                                              \
    _container[handle.idx] = {};                                                                  \
    construct_##name(handle, info);                                                               \
    return handle;                                                                                \
  }                                                                                               \
  uint32_t name##_count() {                                                                       \
    return _alloc.count();                                                                        \
  }                                                                                               \
  inline auto& name##_ref(handle_t handle) {                                                      \
    return _container[handle.idx];                                                                \
  }                                                                                               \
