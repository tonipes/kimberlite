// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/core.h>
#include <kb/hash.h>

#ifdef __cplusplus
extern "C" {
#endif  

typedef struct kb_table {
  uint32_t  capacity;
  uint32_t  count;
  kb_hash*  keys;
  uint32_t* handles;
} kb_table;

KB_API void       kb_table_create       (kb_table* table, uint32_t capacity);
KB_API void       kb_table_destroy      (kb_table* table);
KB_API void       kb_table_copy         (kb_table* dst, const kb_table* src);
KB_API uint32_t   kb_table_count        (kb_table* table);
KB_API void       kb_table_reset        (kb_table* table);
KB_API bool       kb_table_insert       (kb_table* table, kb_hash key, uint32_t handle);
KB_API uint32_t   kb_table_get          (const kb_table* table, kb_hash key);
KB_API kb_hash    kb_table_get_hash     (const kb_table* table, uint32_t handle);
KB_API bool       kb_table_has          (kb_table* table, kb_hash key);
KB_API uint32_t   kb_table_find_index   (const kb_table* table, kb_hash key);
KB_API bool       kb_table_remove       (kb_table* table, kb_hash key);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

//namespace kb {
//  class table: public kb_table {
//  public:
//    table(uint32_t capacity = 0) { kb_table_create(this, capacity); }
//
//    ~table() {
//      kb_table_destroy(this);
//    }
//
//    table(const table& other) {
//      kb_table_copy(this, &other);
//    }
//
//    table& operator=(const table& other) {
//      return *this = table(other);
//    }
//
//    table(table&& other) {
//      *this = other;
//      other.data  = nullptr;
//      other.pos   = 0;
//      other.cap   = 0;
//    }
//
//    table& operator=(table&& other) noexcept {
//      *this = other;
//      other.data  = nullptr;
//      other.pos   = 0;
//      other.cap   = 0;
//      return *this;
//    }
//
//    uint32_t capacity() const {
//      return kb_table_capacity(this);
//    }
//
//    uint32_t count() const {
//      return kb_table_count(this);
//    }
//
//    uint32_t take() {
//      return kb_table_take(this);
//    }
//
//    bool free(uint32_t handle) {
//      return kb_table_free(this, handle);
//    }
//  };
//};

#endif
