#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define KB_RESOURCE_HASHED_FUNC_DECLS(name, handle_t, create_info_t)                              \
  void      kb_##name##_set_name(handle_t handle, const char* name);                              \
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
