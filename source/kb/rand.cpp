// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/rand.h>

static kb_rng default_rng { 34221 };

KB_API uint32_t kb_rand_gen(kb_rng* rng) {
  kb_rng* r = rng == NULL ? &default_rng : rng;

  r->seed ^= r->seed << 17;
  r->seed ^= r->seed >> 13;
  r->seed ^= r->seed << 5;
    
  return r->seed;
}

KB_API float kb_rand(kb_rng* rng) {
  uint32_t rnd = kb_rand_gen(rng) & UINT16_MAX;
  float r = float(rnd) * 1.0f / float(UINT16_MAX);
    
  return r;
}

KB_API float kb_rand_unit(kb_rng* rng) {
  return 2.0f * kb_rand(rng) - 1.0f;
}

KB_API kb_float2 kb_rand_unit_circle(kb_rng* rng) {
  float a = kb_rand_unit(rng) * PI2;
  return { cos(a), sin(a) };
}

KB_API kb_float3 kb_rand_unit_sphere(kb_rng* rng) {
	float r0  = kb_rand_unit(rng) * 2.0f - 1.0f;
	float r1  = kb_rand_unit(rng) * PI2;
	float s1  = sqrt(1.0f - r0 * r1);

  return { s1 * cos(r1), s1 * sin(r1), r0 };
}

KB_API float kb_rand_range(kb_rng* rng, float from, float to) {
  return kb_float_remap(kb_rand(rng), 0.0f, 1.0f, from, to);
}
