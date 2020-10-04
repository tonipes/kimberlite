#pragma once

#include <kb/core.h>
#include <kb/hash.h>


#ifdef __cplusplus
extern "C" {
#endif  

typedef struct kb_table {
  uint32_t  capacity;
  uint32_t  count;
  kb_hash*     keys;
  uint32_t* handles;
} kb_table;

KB_API void       kb_table_create       (kb_table* table, uint32_t capacity);
KB_API void       kb_table_destroy      (kb_table* table);
KB_API void       kb_table_copy         (kb_table* dst, const kb_table* src);

KB_API uint32_t   kb_table_count        (kb_table* table);
KB_API void       kb_table_reset        (kb_table* table);

KB_API bool       kb_table_insert       (kb_table* table, kb_hash key, uint32_t handle);
KB_API uint32_t   kb_table_get          (const kb_table* table, kb_hash key);
KB_API kb_hash       kb_table_get_hash     (kb_table* table, uint32_t handle);
KB_API bool       kb_table_has          (kb_table* table, kb_hash key);
KB_API uint32_t   kb_table_find_index   (const kb_table* table, kb_hash key);

KB_API bool       kb_table_remove       (kb_table* table, kb_hash key);

#ifdef __cplusplus
}
#endif
