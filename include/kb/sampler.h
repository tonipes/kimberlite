#pragma once

#include <kb/math.h>

// c++ implementation
#ifdef __cplusplus

template <uint32_t SampleCount>
struct DataSampler {
  DataSampler() {
    reset();
  }

  void reset() {
    offset = 0;

    kb_memset(values, 0, sizeof(values));

    min = 0.0f;
    max = 0.0f;
    avg = 0.0f;
  }

  void push_sample(float value) {
    values[offset] = value;
    offset = (offset + 1) % SampleCount;

    float n_min =  FLOATMAX;
    float n_max = -FLOATMAX;
    float sum =  0.0f;

    for (uint32_t i = 0; i < SampleCount; ++i) {
      const float val = values[i];
      n_min  = min_scalar(n_min, val);
      n_max  = max_scalar(n_max, val);
      sum += val;
    }

    min = n_min;
    max = n_max;
    avg = sum / SampleCount;
  }

  int32_t offset;
  float values[SampleCount];

  float min;
  float max;
  float avg;
};

#endif
