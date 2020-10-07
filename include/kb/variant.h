#pragma once

#include <kb/core.h>
#include <kb/hash.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kb_variant_type {
  KB_VARIANT_NULL     = 0, 
  KB_VARIANT_STRING   = 1, 
  KB_VARIANT_OBJECT   = 2, 
  KB_VARIANT_ARRAY    = 3, 
  KB_VARIANT_BOOLEAN  = 4, 
  KB_VARIANT_NUMBER   = 5, 
} kb_variant_type;

typedef struct {
  kb_variant_type type;
  struct kb_variant_impl* impl;
} kb_variant;

KB_API void           kb_variant_create_null    (kb_variant* variant);
KB_API void           kb_variant_create_str     (kb_variant* variant, const char* str);
KB_API void           kb_variant_create_obj     (kb_variant* variant, uint32_t capacity);
KB_API void           kb_variant_create_arr     (kb_variant* variant, uint32_t capacity);
KB_API void           kb_variant_create_bool    (kb_variant* variant, bool value);
KB_API void           kb_variant_create_num     (kb_variant* variant, float value);

KB_API void           kb_variant_destroy        (kb_variant* variant);

KB_API void           kb_variant_copy           (kb_variant* dst, const kb_variant* src);
KB_API void           kb_variant_merge          (kb_variant* dst, const kb_variant* src);

KB_API float          kb_variant_get_num        (const kb_variant* variant);
KB_API bool           kb_variant_get_bool       (const kb_variant* variant);
KB_API const char*    kb_variant_get_str        (const kb_variant* variant);

KB_API float          kb_variant_get_num_default  (const kb_variant* variant, float default_value);
KB_API bool           kb_variant_get_bool_default (const kb_variant* variant, bool default_value);
KB_API const char*    kb_variant_get_str_default  (const kb_variant* variant, const char* default_value);

KB_API bool           kb_variant_is_num         (const kb_variant* variant);
KB_API bool           kb_variant_is_bool        (const kb_variant* variant);
KB_API bool           kb_variant_is_str         (const kb_variant* variant);
KB_API bool           kb_variant_is_arr         (const kb_variant* variant);
KB_API bool           kb_variant_is_obj         (const kb_variant* variant);

KB_API uint32_t       kb_variant_obj_capacity   (const kb_variant* variant);
KB_API uint32_t       kb_variant_obj_count      (const kb_variant* variant);
KB_API kb_variant*    kb_variant_obj_at         (kb_variant* variant, const char* key);
KB_API kb_variant*    kb_variant_obj_put        (kb_variant* variant, const char* key);
KB_API kb_variant*    kb_variant_obj_at_hash    (kb_variant* variant, kb_hash key);
KB_API kb_variant*    kb_variant_obj_put_hash   (kb_variant* variant, kb_hash key);
KB_API kb_hash        kb_variant_obj_key_at     (kb_variant* variant, uint32_t index);

KB_API uint32_t       kb_variant_arr_capacity   (const kb_variant* variant);
KB_API uint32_t       kb_variant_arr_count      (const kb_variant* variant);
KB_API kb_variant*    kb_variant_arr_put        (kb_variant* variant);
KB_API kb_variant*    kb_variant_arr_at         (kb_variant* variant, uint32_t index);

KB_API void           kb_variant_dump           (const kb_variant* variant);

#ifdef __cplusplus
}
#endif
