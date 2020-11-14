// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/table.h>

#include <kb/alloc.h>
#include <kb/crt.h>

KB_API void kb_table_create(kb_table* table, uint32_t capacity) {
  KB_ASSERT_NOT_NULL(table);

  table->handles  = KB_DEFAULT_ALLOC_TYPE(uint32_t, capacity);
  table->keys     = KB_DEFAULT_ALLOC_TYPE(kb_hash,  capacity);
  table->capacity = capacity;
  
  kb_table_reset(table);
}

KB_API void kb_table_destroy(kb_table* table) {
  KB_ASSERT_NOT_NULL(table);

  if (table->capacity == 0) return;

  KB_DEFAULT_FREE(table->handles);
  KB_DEFAULT_FREE(table->keys);

  kb_memset(table, '\0', sizeof(kb_table));
}

KB_API void kb_table_reset(kb_table* table) {
  KB_ASSERT_NOT_NULL(table);

  for (uint32_t i = 0; i < table->capacity; i++) {
    table->handles[i] = UINT32_MAX;
  }
  table->count = 0;
}

KB_API uint32_t kb_table_count(kb_table* table) {
  KB_ASSERT_NOT_NULL(table);

  return table->count;
}

KB_API bool kb_table_insert(kb_table* table, kb_hash key, uint32_t handle) {
  KB_ASSERT_NOT_NULL(table);

  if (table->capacity == 0) return false;

  const uint32_t first_idx = key % table->capacity;
  uint32_t idx = first_idx;
  
  do {
    if (table->handles[idx] == UINT32_MAX) {
      table->keys[idx]    = key;
      table->handles[idx] = handle;
      table->count++;
      return true;
    }

    if (table->keys[idx] == key) { return false; }
    
    idx = (idx + 1) % table->capacity;

  } while (idx != first_idx);

  return false;
}

KB_API uint32_t kb_table_get(const kb_table* table, kb_hash key) {
  KB_ASSERT_NOT_NULL(table);

  const uint32_t idx = kb_table_find_index(table, key);
  if (idx != UINT32_MAX) {
    return table->handles[idx];
  }

  return UINT32_MAX;
}

KB_API kb_hash kb_table_get_hash(const kb_table* table, uint32_t handle) {
  KB_ASSERT_NOT_NULL(table);

  for (uint32_t idx = 0; idx < table->capacity; ++idx) {
    if (table->handles[idx] == handle) {
      return table->keys[idx];
    }
  }

  return UINT32_MAX;
}

KB_API uint32_t kb_table_find_index(const kb_table* table, kb_hash key) {
  KB_ASSERT_NOT_NULL(table);

  if (table->capacity == 0) return UINT32_MAX;

  const uint32_t first_idx = key % table->capacity;
  uint32_t idx = first_idx;

  do {
    if (table->handles[idx] == UINT32_MAX) {
      return UINT32_MAX;
    }

    if (table->keys[idx] == key) {
      return idx;
    }

    idx = (idx + 1) % table->capacity;

  } while (idx != first_idx);
  
  return UINT32_MAX;
}

KB_API bool kb_table_has(kb_table* table, kb_hash key) {
  KB_ASSERT_NOT_NULL(table);

  const uint32_t idx = kb_table_find_index(table, key);

  if (idx != UINT32_MAX) {
    return true;
  }

  return false;
}

KB_API void kb_table_remove_index(kb_table* table, uint32_t index) {
  KB_ASSERT_NOT_NULL(table);

  if (table->capacity == 0) return;

  table->handles[index] = UINT32_MAX;
  table->count--;

  const uint32_t s = (index + 1) % table->capacity;

  for (uint32_t i = s; table->handles[i] != UINT32_MAX; i = (i + 1) % table->capacity) {
    if (table->handles[i] != UINT32_MAX) {
      const kb_hash key = table->keys[i];

      if (i != kb_table_find_index(table, key)) {
        const uint16_t handle = table->handles[i];
        table->handles[i] = UINT32_MAX;
        table->count--;
        kb_table_insert(table, key, handle);
      }
    }
  }
}

KB_API bool kb_table_remove(kb_table* table, kb_hash key) {
  KB_ASSERT_NOT_NULL(table);

  uint32_t idx = kb_table_find_index(table, key);

  if (idx != UINT32_MAX) {
    kb_table_remove_index(table, idx);
    return true;
  }

  return false;
}

KB_API void kb_table_copy(kb_table* dst, const kb_table* src) {
  KB_ASSERT_NOT_NULL(dst);
  KB_ASSERT_NOT_NULL(src);

  if (src->capacity == 0) return;

  kb_table_create(dst, src->capacity);

  kb_memcpy(dst->keys,    src->keys,    sizeof(kb_hash)      * src->capacity);
  kb_memcpy(dst->handles, src->handles, sizeof(uint32_t)  * src->capacity);

  dst->count = src->count;
}
