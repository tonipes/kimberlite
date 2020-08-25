#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define KB_RESOURCE_HASHED_FUNC_DECLS(name, handle_t, create_info_t)                              \
  void set_##name##_hash(Hash key, handle_t handle);                                              \
  void remove_##name##_hash(handle_t handle);                                                     \
  bool has_##name(Hash key);                                                                      \
  handle_t get_##name(Hash key);                                                                  \
  handle_t get_or_allocate_##name(Hash key);                                                      \
  handle_t create_named_##name(Hash key, const create_info_t info);                               \

#define KB_RESOURCE_CORE_FUNC_DECLS(name, handle_t, create_info_t)                                \
  handle_t allocate_##name();                                                                     \
  handle_t create_##name(const create_info_t info);                                               \
  void destroy_##name(handle_t handle);                                                           \
  uint32_t name##_count();                                                                        \

#ifdef __cplusplus
}
#endif
