// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/core.h>

#ifdef __cplusplus
extern "C" {
#endif  

typedef struct kb_array {
  uint64_t  elem_size;
  uint64_t  cap;
  uint64_t  pos;
  void*     data;
} kb_array;

KB_API void     kb_array_create     (kb_array* array, uint64_t elem_size, uint64_t capacity);
KB_API void     kb_array_destroy    (kb_array* array);
KB_API void     kb_array_reset      (kb_array* array);
KB_API void     kb_array_copy       (kb_array* dst, const kb_array* src);

KB_API void*    kb_array_begin      (kb_array* array);
KB_API void*    kb_array_back       (kb_array* array);
KB_API void*    kb_array_end        (kb_array* array);
KB_API uint64_t kb_array_count      (kb_array* array);
KB_API uint64_t kb_array_capacity   (kb_array* array);
KB_API void     kb_array_reserve    (kb_array* array, uint64_t size);
KB_API void     kb_array_resize     (kb_array* array, uint64_t size);
KB_API void*    kb_array_get        (kb_array* array, uint64_t index);
KB_API void     kb_array_set        (kb_array* array, uint64_t index);
KB_API void     kb_array_push_back  (kb_array* array, void* data);
KB_API void     kb_array_pop_back   (kb_array* array);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace kb {
  template <typename T>
  class array: public kb_array {
  public:
    array(uint64_t capacity = 0) { kb_array_create(this, sizeof(T), capacity); }

    ~array() { kb_array_destroy(this); }
    
    array(const array& other) { kb_array_copy(this, &other); }
    array& operator=(const array& other) { return *this = array(other); }
 
    array(array&& other) {
      *this = other;
      other.data = nullptr;
      other.pos  = 0;
      other.cap  = 0;
    } 
    
    array& operator=(array&& other) noexcept {
      *this = other;
      other.data = nullptr;
      other.pos  = 0;
      other.cap  = 0;
      return *this;
    }
    
    uint64_t capacity ()              const   { return kb_array_capacity(this);       }
    uint64_t count    ()              const   { return kb_array_count(this);          }
    void     reserve  (uint64_t size)         { return kb_array_reserve(this, size);  }
    void     resize   (uint64_t size)         { return kb_array_resize(this, size);   }
  };
};

#endif
