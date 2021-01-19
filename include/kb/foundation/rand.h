// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include "core.h"
#include "math.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kb_rng {
  uint32_t seed;
} kb_rng;

KB_API uint32_t   kb_rand_gen          (kb_rng* rng);
KB_API float      kb_rand              (kb_rng* rng);
KB_API float      kb_rand_unit         (kb_rng* rng);
KB_API float      kb_rand_range        (kb_rng* rng, float from, float to);
KB_API kb_float2  kb_rand_unit_circle  (kb_rng* rng);
KB_API kb_float3  kb_rand_unit_sphere  (kb_rng* rng);

#ifdef __cplusplus
}
#endif
