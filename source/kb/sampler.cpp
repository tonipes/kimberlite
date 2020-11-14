// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/sampler.h>

#include <kb/alloc.h>
#include <kb/crt.h>

KB_API void kb_sampler_create(kb_sampler* sampler, uint32_t capacity) {
  KB_ASSERT_NOT_NULL(sampler);

  sampler->values   = KB_DEFAULT_ALLOC_TYPE(float, capacity);
  sampler->capacity = capacity;
  kb_sampler_reset(sampler);
}

KB_API void kb_sampler_reset(kb_sampler* sampler) {
  KB_ASSERT_NOT_NULL(sampler);

  sampler->count    = 0;
  sampler->offset   = 0;
  sampler->min      = 0;
  sampler->max      = 0;
  sampler->avg      = 0;

  kb_memset(sampler->values, 0, sizeof(float) * sampler->capacity);
}

KB_API void kb_sampler_destroy(kb_sampler* sampler) {
  KB_ASSERT_NOT_NULL(sampler);

  KB_DEFAULT_FREE(sampler->values);
}

KB_API void kb_sampler_push(kb_sampler* sampler, float value) {
  KB_ASSERT_NOT_NULL(sampler);

  sampler->count = MIN(sampler->count + 1, sampler->capacity);

  sampler->values[sampler->offset] = value;
  sampler->offset = (sampler->offset + 1) % sampler->capacity;

  float n_min =  FLOATMAX;
  float n_max = -FLOATMAX;
  float sum =  0.0f;
    
  for (uint32_t i = 0; i < sampler->count; ++i) {
    const float val = sampler->values[i];
    n_min = min_scalar(n_min, val);
    n_max = max_scalar(n_max, val);
    sum += val;
  }

  sampler->min = n_min;
  sampler->max = n_max;
  sampler->avg = sum / sampler->count;
}
