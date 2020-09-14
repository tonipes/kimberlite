#pragma once

#ifdef __cplusplus
extern "C" {
#endif  

#include "core.h"

typedef struct {
  uint32_t  capacity;
  uint32_t  count;
  Hash*     keys;
  uint32_t* handles;
} Table;

KB_API void       kb_table_create       (Table* table, uint32_t capacity);
KB_API void       kb_table_destroy      (Table* table);
KB_API void       kb_table_copy         (Table* dst, const Table* src);

KB_API uint32_t   kb_table_count        (Table* table);
KB_API void       kb_table_reset        (Table* table);

KB_API bool       kb_table_insert       (Table* table, Hash key, uint32_t handle);
KB_API uint32_t   kb_table_get          (Table* table, Hash key);
KB_API bool       kb_table_has          (Table* table, Hash key);
KB_API uint32_t   kb_table_find_index   (Table* table, Hash key);

KB_API bool       kb_table_remove       (Table* table, Hash key);


#ifdef __cplusplus
}
#endif
