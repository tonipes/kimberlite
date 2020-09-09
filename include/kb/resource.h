#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define KB_RESOURCE_HASHED_FUNC_DECLS(name, handle_t, create_info_t)                              \
  void      kb_##name##_set_hash(Hash key, handle_t handle);                                      \
  void      kb_##name##_remove_hash(handle_t handle);                                             \
  bool      kb_##name##_has(Hash key);                                                            \
  handle_t  kb_##name##_get(Hash key);                                                            \
  handle_t  kb_##name##_get_or_allocate(Hash key);                                                \
  handle_t  kb_##name##_create_named(Hash key, const create_info_t info);                         \

#define KB_RESOURCE_CORE_FUNC_DECLS(name, handle_t, create_info_t)                                \
  handle_t kb_##name##_allocate();                                                                \
  handle_t kb_##name##_create(const create_info_t info);                                          \
  void     kb_##name##_destroy(handle_t handle);                                                  \
  uint32_t kb_##name##_count();                                                                   \

#ifdef __cplusplus
}
#endif
