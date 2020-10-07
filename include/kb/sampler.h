#pragma once

#include <kb/math.h>

#ifdef __cplusplus
extern "C" {
#endif  

typedef struct kb_sampler {
  uint32_t  capacity;
  int32_t   offset;
  float*    values;
  float     min;
  float     max;
  float     avg;
} kb_sampler;

KB_API void kb_sampler_create   (kb_sampler* sampler, uint32_t capacity);
KB_API void kb_sampler_reset    (kb_sampler* sampler);
KB_API void kb_sampler_destroy  (kb_sampler* sampler);
KB_API void kb_sampler_push     (kb_sampler* sampler, float value);

#ifdef __cplusplus
}
#endif

// c++ implementation
#ifdef __cplusplus

template <uint32_t SampleCount>
kb_sampler kb_sampler_construct() {
  kb_sampler sampler;
  kb_sampler_create(&sampler, SampleCount);
  return sampler;
}

#endif
