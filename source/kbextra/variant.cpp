// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kbextra/variant.h>

#include <kb/table.h>
#include <kb/alloc.h>
#include <kb/crt.h>

typedef struct kb_variant_impl {
  union {
    float         number;
    bool          boolean;
    
    // String
    struct {
      char*       string;
    };
    
    // Array
    struct {
      uint32_t    arr_capacity;
      kb_variant* arr_variants;
      uint32_t    arr_count;
    };
    
    // Object
    struct {
      uint32_t    obj_capacity;
      kb_variant* obj_variants;
      kb_table    obj_table;
      uint32_t    obj_count;
    };

  };
} kb_variant_impl;

KB_API void kb_variant_create_null(kb_variant* variant) {
  KB_ASSERT_NOT_NULL(variant);

  variant->type = KB_VARIANT_NULL;
  variant->impl = KB_DEFAULT_ALLOC_TYPE(kb_variant_impl, 1);
}

KB_API void kb_variant_create_str(kb_variant* variant, const char* str) {
  KB_ASSERT_NOT_NULL(variant);

  variant->type = KB_VARIANT_STRING;
  variant->impl = KB_DEFAULT_ALLOC_TYPE(kb_variant_impl, 1);
  variant->impl->string = kb_strdup(str);
}

KB_API void kb_variant_create_obj(kb_variant* variant, uint32_t capacity) {
  KB_ASSERT_NOT_NULL(variant);

  variant->type = KB_VARIANT_OBJECT;
  variant->impl = KB_DEFAULT_ALLOC_TYPE(kb_variant_impl, 1);

  variant->impl->obj_capacity = capacity;
  variant->impl->obj_variants = KB_DEFAULT_ALLOC_TYPE(kb_variant, capacity);

  variant->impl->obj_count    = 0;
  
  kb_table_create(&variant->impl->obj_table, capacity);
}

KB_API void kb_variant_create_arr(kb_variant* variant, uint32_t capacity) {
  KB_ASSERT_NOT_NULL(variant);

  variant->type = KB_VARIANT_ARRAY;
  variant->impl = KB_DEFAULT_ALLOC_TYPE(kb_variant_impl, 1);

  variant->impl->arr_capacity = capacity;
  variant->impl->arr_variants = KB_DEFAULT_ALLOC_TYPE(kb_variant, capacity);
  variant->impl->arr_count    = 0;
}

KB_API void kb_variant_create_bool(kb_variant* variant, bool value) {
  KB_ASSERT_NOT_NULL(variant);

  variant->type = KB_VARIANT_BOOLEAN;
  variant->impl = KB_DEFAULT_ALLOC_TYPE(kb_variant_impl, 1);
  variant->impl->boolean = value;
}

KB_API void kb_variant_create_num(kb_variant* variant, float value) {
  KB_ASSERT_NOT_NULL(variant);

  variant->type = KB_VARIANT_NUMBER;
  variant->impl = KB_DEFAULT_ALLOC_TYPE(kb_variant_impl, 1);
  variant->impl->number = value;
}

KB_API void kb_variant_destroy(kb_variant* variant) {
  KB_ASSERT_NOT_NULL(variant);

  if (kb_variant_is_str(variant)) {
    KB_DEFAULT_FREE(variant->impl->string);
  }

  if (kb_variant_is_arr(variant)) {
    for (uint32_t i = 0; i < kb_variant_arr_count(variant); ++i) {
      kb_variant_destroy(kb_variant_arr_at(variant, i));
    }
    KB_DEFAULT_FREE(variant->impl->arr_variants);
  }

  if (kb_variant_is_obj(variant)) {
    for (uint32_t i = 0; i < kb_variant_obj_count(variant); ++i) {
      kb_variant_destroy(kb_variant_obj_at_hash(variant, kb_variant_obj_key_at(variant, i)));
    }
    KB_DEFAULT_FREE(variant->impl->obj_variants);
    kb_table_destroy(&variant->impl->obj_table);
  }

  KB_DEFAULT_FREE(variant->impl);
  
  kb_memset(variant, '\0', sizeof(kb_variant));
}

KB_API void kb_variant_copy(kb_variant* dst, const kb_variant* src) {
  KB_ASSERT_NOT_NULL(dst);
  KB_ASSERT_NOT_NULL(src);

  switch (src->type) {
    case KB_VARIANT_NULL:     kb_variant_create_null  (dst); break;
    case KB_VARIANT_STRING:   kb_variant_create_str   (dst, src->impl->string); break;
    case KB_VARIANT_BOOLEAN:  kb_variant_create_bool  (dst, src->impl->boolean); break;
    case KB_VARIANT_NUMBER:   kb_variant_create_num   (dst, src->impl->number); break;
    
    case KB_VARIANT_OBJECT: {
      kb_variant_create_obj(dst, src->impl->obj_capacity);

      for (uint32_t i = 0; i < src->impl->obj_count; i++) {
        kb_variant* nvar = kb_variant_obj_put_hash(dst, kb_variant_obj_key_at(src, i));
        kb_variant_copy(nvar, &src->impl->obj_variants[i]);
      }    
    } break;

    case KB_VARIANT_ARRAY: {
      kb_variant_create_arr(dst, src->impl->arr_capacity);
    
      for (uint32_t i = 0; i < src->impl->arr_count; i++) {
        kb_variant* obj = kb_variant_arr_put(dst);
        kb_variant_copy(obj, &src->impl->arr_variants[i]);
      }

    } break;
  }
}

// TODO: Incomplete.
KB_API void kb_variant_merge(kb_variant* dst, const kb_variant* src) {
  KB_ASSERT_NOT_NULL(dst);
  KB_ASSERT_NOT_NULL(src);

  KB_ASSERT(kb_variant_is_obj(src), "Merge src is not an object. Only object to object merge is implemented.");
  KB_ASSERT(kb_variant_is_obj(dst), "Merge dst is not an object. Only object to object merge is implemented.");

  // for (uint32_t i = 0; i < src->impl->obj_count; i++) {
  //   // kb_hash key = kb_table_get_hash(&src->impl->obj_table, i);

  //   // const kb_variant*  src_obj = &src->impl->obj_variants[i];
  //   // kb_variant*        dst_obj = kb_variant_obj_put_hash(dst, key);

  //   kb_variant_copy(dst, src);
  // }

  kb_variant_copy(dst, src);

}

KB_API float kb_variant_get_num(const kb_variant* variant) {
  KB_ASSERT_NOT_NULL(variant);
  KB_ASSERT(kb_variant_is_num(variant), "kb_variant is not a number");

  return variant->impl->number;
}

KB_API bool kb_variant_get_bool(const kb_variant* variant) {
  KB_ASSERT_NOT_NULL(variant);
  KB_ASSERT(kb_variant_is_bool(variant), "kb_variant is not a boolean");
  
  return variant->impl->boolean;
}

KB_API const char* kb_variant_get_str(const kb_variant* variant) {
  KB_ASSERT_NOT_NULL(variant);
  KB_ASSERT(kb_variant_is_str(variant), "kb_variant is not a string");
  
  return variant->impl->string;
}

KB_API float kb_variant_get_num_default(const kb_variant* variant, float default_value) {
  KB_ASSERT_NOT_NULL(variant);

  if (kb_variant_is_num(variant)) return kb_variant_get_num(variant);

  return default_value;
}

KB_API bool kb_variant_get_bool_default(const kb_variant* variant, bool default_value) {
  KB_ASSERT_NOT_NULL(variant);

  if (kb_variant_is_bool(variant)) return kb_variant_get_bool(variant);

  return default_value;
}

KB_API const char* kb_variant_get_str_default(const kb_variant* variant, const char* default_value) {
  KB_ASSERT_NOT_NULL(variant);
  
  if (kb_variant_is_str(variant)) return kb_variant_get_str(variant);
  
  return default_value;
}

KB_API kb_variant* kb_variant_obj_at(kb_variant* variant, const char* key) {
  KB_ASSERT_NOT_NULL(variant);

  return kb_variant_obj_at_hash(variant, kb_hash_string(key));
}

KB_API kb_hash kb_variant_obj_key_at(const kb_variant* variant, uint32_t index) {
  KB_ASSERT_NOT_NULL(variant);
  KB_ASSERT(kb_variant_is_obj(variant), "kb_variant is not an object");

  return kb_table_get_hash(&variant->impl->obj_table, index);
}

KB_API kb_variant* kb_variant_obj_at_hash(kb_variant* variant, kb_hash key) {
  KB_ASSERT_NOT_NULL(variant);
  KB_ASSERT(kb_variant_is_obj(variant), "kb_variant is not an object");

  uint32_t idx = kb_table_get(&variant->impl->obj_table, key);

  if (idx >= variant->impl->obj_capacity) return NULL;

  return &variant->impl->obj_variants[idx];
}

KB_API kb_variant* kb_variant_obj_put_hash(kb_variant* variant, kb_hash key) {
  KB_ASSERT_NOT_NULL(variant);
  KB_ASSERT(kb_variant_is_obj(variant), "kb_variant is not an object");

  uint32_t existing_idx = kb_table_get(&variant->impl->obj_table, key);

  if (existing_idx < variant->impl->obj_capacity) {
    // Valid existing found. 
    // Destroy and use existing slot
    kb_variant* obj = &variant->impl->obj_variants[existing_idx];
    kb_variant_destroy(obj);
    return obj;
  } else {
    // Take new slot
    uint32_t idx = variant->impl->obj_count++;
    kb_table_insert(&variant->impl->obj_table, key, idx);
    return &variant->impl->obj_variants[idx];
  }
}

KB_API kb_variant* kb_variant_obj_put(kb_variant* variant, const char* key) {
  KB_ASSERT_NOT_NULL(variant);

  return kb_variant_obj_put_hash(variant, kb_hash_string(key));  
}

KB_API uint32_t kb_variant_obj_capacity(const kb_variant* variant) {
  KB_ASSERT_NOT_NULL(variant);
  KB_ASSERT(kb_variant_is_obj(variant), "kb_variant is not an object");
  return variant->impl->obj_capacity;
}

KB_API uint32_t kb_variant_obj_count(const kb_variant* variant) {
  KB_ASSERT_NOT_NULL(variant);
  KB_ASSERT(kb_variant_is_obj(variant), "kb_variant is not an object");
  return variant->impl->obj_count;
}

KB_API kb_variant* kb_variant_arr_at(kb_variant* variant, uint32_t index) {
  KB_ASSERT_NOT_NULL(variant);
  KB_ASSERT(kb_variant_is_arr(variant), "kb_variant is not an array");
  
  if (index >= variant->impl->arr_capacity) return NULL;
  
  return &variant->impl->arr_variants[index];
}

KB_API kb_variant* kb_variant_arr_put(kb_variant* variant) {
  KB_ASSERT_NOT_NULL(variant);
  KB_ASSERT(kb_variant_is_arr(variant), "kb_variant is not an array");

  uint32_t idx = variant->impl->arr_count++;
  return &variant->impl->arr_variants[idx];
}

KB_API uint32_t kb_variant_arr_capacity(const kb_variant* variant) {
  KB_ASSERT_NOT_NULL(variant);
  KB_ASSERT(kb_variant_is_arr(variant), "kb_variant is not an array");

  return variant->impl->arr_capacity;  
}

KB_API uint32_t kb_variant_arr_count(const kb_variant* variant) {
  KB_ASSERT_NOT_NULL(variant);
  KB_ASSERT(kb_variant_is_arr(variant), "kb_variant is not an array");

  return variant->impl->arr_count;
}

KB_API bool kb_variant_is_num(const kb_variant* variant) {
  KB_ASSERT_NOT_NULL(variant);

  return variant->type == KB_VARIANT_NUMBER;
}

KB_API bool kb_variant_is_bool(const kb_variant* variant) {
  KB_ASSERT_NOT_NULL(variant);

  return variant->type == KB_VARIANT_BOOLEAN;
}

KB_API bool kb_variant_is_str(const kb_variant* variant) {
  KB_ASSERT_NOT_NULL(variant);

  return variant->type == KB_VARIANT_STRING;
}

KB_API bool kb_variant_is_arr(const kb_variant* variant) {
  KB_ASSERT_NOT_NULL(variant);

  return variant->type == KB_VARIANT_ARRAY;
}

KB_API bool kb_variant_is_obj(const kb_variant* variant) {
  KB_ASSERT_NOT_NULL(variant);

  return variant->type == KB_VARIANT_OBJECT;
}


// Dump

static const int INDENT_WIDTH = 4;

void print_indent(int indent) {
  kb_printf("\n");
  for (int i = 0; i < (indent - 1) * INDENT_WIDTH; ++i) {
    kb_printf(" ");
  }
}

inline void write_str(const kb_variant* variant) {
  KB_ASSERT_NOT_NULL(variant);

  switch (variant->type) {
    case KB_VARIANT_NULL:    kb_printf("null"); break;
    case KB_VARIANT_BOOLEAN: kb_printf(variant->impl->boolean ? "true" : "false"); break;
    case KB_VARIANT_STRING:  kb_printf(variant->impl->string); break;
    case KB_VARIANT_ARRAY:   kb_printf("array"); break;
    case KB_VARIANT_OBJECT:  kb_printf("object"); break;
    case KB_VARIANT_NUMBER:  kb_printf("%.6f", variant->impl->number); break;
  }
}


void print_char(char c) {
  switch (c) {
    case '"'  : kb_printf("\\\""); break;
    case '\\' : kb_printf("\\\\"); break;
    case '/'  : kb_printf("\\/" ); break;
    case '\b' : kb_printf("\\b" ); break;
    case '\f' : kb_printf("\\f" ); break;
    case '\n' : kb_printf("\\n" ); break;
    case '\r' : kb_printf("\\r" ); break;
    case '\t' : kb_printf("\\t" ); break;
    default: {
      if (static_cast<unsigned char>(c) < 0x20 || c == 0x7f) {
        kb_printf("\\u%04x", c & 0xff);
      } else {
        kb_printf("%c", c); 
      } break;
    }
  }
}

void print_str(const char* str) {
  for (uint32_t i = 0; i < kb_strlen(str); i++) {
    print_char(str[i]);    
  }
}

KB_API void dump(const kb_variant* variant, int indent) {
  KB_ASSERT_NOT_NULL(variant);

  switch (variant->type) {
    case KB_VARIANT_STRING: {
      print_str(variant->impl->string);
    } break;

    case KB_VARIANT_ARRAY: {
      if (indent != -1) ++indent;
      for (uint32_t i = 0; i < variant->impl->arr_count; i++) {
        if (indent != -1) print_indent(indent);
        
        kb_printf("-");
        if (indent != -1) kb_printf(" "); 
  
        dump(&variant->impl->arr_variants[i], indent);
      }
    } break;
    
    case KB_VARIANT_OBJECT: {
      if (indent != -1) ++indent;
      for (uint32_t i = 0; i < variant->impl->obj_count; i++) {
        if (indent != -1) print_indent(indent);        
        kb_printf("%u", kb_table_get_hash(&variant->impl->obj_table, i));
        kb_printf(":");
        
        if (indent != -1) kb_printf(" "); 
  
        dump(&variant->impl->obj_variants[i], indent);
      }
    } break;

    default: {
      write_str(variant);
    } break;
  }

  if (indent == 1) {
    kb_printf("\n");
  }
  
  return;
}

KB_API void kb_variant_dump(const kb_variant* variant) {
  KB_ASSERT_NOT_NULL(variant);

  dump(variant, 0);
}
