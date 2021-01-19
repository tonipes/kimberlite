// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif  

typedef struct kb_freelist {
  uint32_t*   data;
  uint32_t    pos;
  uint32_t    cap;
} kb_freelist;

KB_API void       kb_freelist_create        (kb_freelist* freelist, uint32_t capacity);
KB_API void       kb_freelist_destroy       (kb_freelist* freelist);
KB_API void       kb_freelist_reset         (kb_freelist* freelist);
KB_API void       kb_freelist_copy          (kb_freelist* dst, const kb_freelist* src);

KB_API void       kb_freelist_reset         (kb_freelist* freelist);
KB_API uint32_t   kb_freelist_take          (kb_freelist* freelist);
KB_API bool       kb_freelist_free          (kb_freelist* freelist, uint32_t handle);
KB_API uint32_t*  kb_freelist_get_sparse    (const kb_freelist* freelist);
KB_API uint32_t*  kb_freelist_get_dense     (const kb_freelist* freelist);
KB_API uint32_t   kb_freelist_count         (const kb_freelist* freelist);
KB_API uint32_t   kb_freelist_capacity      (const kb_freelist* freelist);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace kb {
  class freelist: public kb_freelist {
  public:
    freelist(uint32_t capacity = 0) { kb_freelist_create(this, capacity); }

    ~freelist() {
      kb_freelist_destroy(this);
    }
    
    freelist(const freelist& other) {
      kb_freelist_copy(this, &other);
    }
    
    freelist& operator=(const freelist& other) {
      return *this = freelist(other);
    }
 
    freelist(freelist&& other) {
      *this = other;
      other.data  = nullptr;
      other.pos   = 0;
      other.cap   = 0;
    }
    
    freelist& operator=(freelist&& other) noexcept {
      *this = other;
      other.data  = nullptr;
      other.pos   = 0;
      other.cap   = 0;
      return *this;
    }
    
    uint32_t capacity() const {
      return kb_freelist_capacity(this);
    }

    uint32_t count() const {
      return kb_freelist_count(this);
    }
    
    uint32_t take() {
      return kb_freelist_take(this);
    }

    bool free(uint32_t handle) {
      return kb_freelist_free(this, handle);
    }
    
    uint32_t* get_dense() const {
      return kb_freelist_get_dense(this);
    }
    
    uint32_t* get_sparse() const {
      return kb_freelist_get_sparse(this);
    }
  };
};

#endif
