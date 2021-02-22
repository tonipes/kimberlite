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
KB_API uint64_t kb_array_count      (const kb_array* array);
KB_API uint64_t kb_array_capacity   (const kb_array* array);
KB_API void     kb_array_reserve    (kb_array* array, uint64_t size);
KB_API void     kb_array_resize     (kb_array* array, uint64_t size);
KB_API void*    kb_array_at         (const kb_array* array, uint64_t index);
KB_API void     kb_array_push_back  (kb_array* array, void* data);
KB_API void     kb_array_pop_back   (kb_array* array);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <initializer_list>

namespace kb {
  template <typename T>
  class array: public kb_array {
  public:
    array(uint64_t capacity = 0) {
      kb_array_create(this, sizeof(T), capacity);
    }
    
    array(std::initializer_list<T> ilist): array(ilist.size()) {
      for (const T& v : ilist) {
        this->push_back(v);
      }
    }
    
    ~array() {
      for (uint32_t i = 0; i < this->pos; ++i) {
        ((T*)kb_array_at(this, i))->~T();
      }
      
      kb_array_destroy(this);
    }
    
    array(const array& other): array(other.capacity()) {
      for (uint32_t i = 0; i < other.pos; ++i) {
        this->push_back(other.at(i));
      }
    }
        
    array& operator=(array other) noexcept {
      std::swap(data, other.data);
      std::swap(pos, other.pos);
      std::swap(cap, other.cap);
      
      return *this;
    }

    uint64_t capacity() const {
      return kb_array_capacity(this);
    }
    
    void push_back(const T& value) {
      resize(count() + 1);
      (*this)[count() - 1] = value;
    }
    
    uint64_t count() const {
      return kb_array_count(this);
    }
    
    void reserve(uint64_t size) {
      return kb_array_reserve(this, size);
    }
    
    void resize(uint64_t size) {
      return kb_array_resize(this, size);
    }
    
    T& operator[](uint64_t idx) {
      return *(T*) kb_array_at(this, idx);
    }

    const T& operator[](uint64_t idx) const {
      return *(const T*) kb_array_at(this, idx);
    }

    T& at(uint64_t idx) {
      return (*this)[idx];
    }

    const T& at(uint64_t idx) const {
      return (*this)[idx];
    }

    T& back() {
      return *(T*) kb_array_back(this);
    }

    const T& back() const {
      return *(T*) kb_array_back(this);
    }

  };
};

#endif
