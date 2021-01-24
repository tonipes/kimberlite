// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include "math.h"

#ifdef __cplusplus
extern "C" {
#endif  

typedef struct kb_sampler {
  uint32_t  capacity;
  uint32_t  count;
  int32_t   offset;
  float*    values;
  float     min;
  float     max;
  float     avg;
} kb_sampler;

KB_API void kb_sampler_create   (kb_sampler* sampler, uint32_t capacity);
KB_API void kb_sampler_reset    (kb_sampler* sampler);
KB_API void kb_sampler_copy     (kb_sampler* dst, const kb_sampler* src);

KB_API void kb_sampler_destroy  (kb_sampler* sampler);
KB_API void kb_sampler_push     (kb_sampler* sampler, float value);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

template <uint32_t SampleCount>
kb_sampler kb_sampler_construct() {
  kb_sampler sampler;
  kb_sampler_create(&sampler, SampleCount);
  return sampler;
}

namespace kb {
  class sampler: public kb_sampler {
  public:
    sampler(uint32_t capacity) {
      kb_sampler_create(this, capacity);
    }
    
    ~sampler() {
      kb_sampler_destroy(this);
    }
    
    sampler(const sampler& other) {
      kb_sampler_copy(this, &other);
    }
    
    sampler& operator=(const sampler& other) {
      kb_sampler_copy(this, &other);
      return *this;
    }
    
    sampler(sampler&& other) {
      *this = other;

      kb_sampler_reset(&other);

      other.values = nullptr;
    }
    
    sampler& operator=(sampler&& other) noexcept {
      *this = other;

      kb_sampler_reset(&other);

      other.values = nullptr;
      
      return *this;
    }
    
    void push(float value) {
      kb_sampler_push(this, value);
    }
    
    float avg() const {
      return ((kb_sampler*) this)->avg;
    }
    
    float min() const {
      return ((kb_sampler*) this)->min;
    }
    
    float max() const {
      return ((kb_sampler*) this)->max;
    }
  };
}

#endif
