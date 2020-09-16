#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "core.h"

typedef enum {
  KB_VARIANT_NULL     = 0, 
  KB_VARIANT_STRING   = 1, 
  KB_VARIANT_OBJECT   = 2, 
  KB_VARIANT_ARRAY    = 3, 
  KB_VARIANT_BOOLEAN  = 4, 
  KB_VARIANT_NUMBER   = 5, 
} VariantType;

typedef struct {
  VariantType type;
  struct VariantImpl* impl;
} Variant;

KB_API void           kb_variant_create_null    (Variant* variant);
KB_API void           kb_variant_create_str     (Variant* variant, const char* str);
KB_API void           kb_variant_create_obj     (Variant* variant, uint32_t capacity);
KB_API void           kb_variant_create_arr     (Variant* variant, uint32_t capacity);
KB_API void           kb_variant_create_bool    (Variant* variant, bool value);
KB_API void           kb_variant_create_num     (Variant* variant, float value);

KB_API void           kb_variant_destroy        (Variant* variant);

KB_API void           kb_variant_copy           (Variant* dst, const Variant* src);
KB_API void           kb_variant_merge          (Variant* dst, const Variant* src);

KB_API float          kb_variant_get_num        (const Variant* variant);
KB_API bool           kb_variant_get_bool       (const Variant* variant);
KB_API const char*    kb_variant_get_str        (const Variant* variant);

KB_API bool           kb_variant_is_num         (const Variant* variant);
KB_API bool           kb_variant_is_bool        (const Variant* variant);
KB_API bool           kb_variant_is_str         (const Variant* variant);
KB_API bool           kb_variant_is_arr         (const Variant* variant);
KB_API bool           kb_variant_is_obj         (const Variant* variant);

KB_API uint32_t       kb_variant_obj_capacity   (const Variant* variant);
KB_API uint32_t       kb_variant_obj_count      (const Variant* variant);
KB_API Variant*       kb_variant_obj_at         (Variant* variant, const char* key);
KB_API Variant*       kb_variant_obj_put        (Variant* variant, const char* key);
KB_API Variant*       kb_variant_obj_at_hash    (Variant* variant, Hash key);
KB_API Variant*       kb_variant_obj_put_hash   (Variant* variant, Hash key);

KB_API uint32_t       kb_variant_arr_capacity   (const Variant* variant);
KB_API uint32_t       kb_variant_arr_count      (const Variant* variant);
KB_API Variant*       kb_variant_arr_put        (Variant* variant);
KB_API Variant*       kb_variant_arr_at         (Variant* variant, uint32_t index);

KB_API void           kb_variant_dump           (const Variant* variant);

#ifdef __cplusplus
}
#endif
