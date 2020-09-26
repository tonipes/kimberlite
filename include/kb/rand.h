#pragma once

#include <kb/api.h>
#include <kb/math.h>

#ifdef __cplusplus
extern "C" {
#endif

KB_API uint32_t  kb_rand_gen();
KB_API float     kb_rand();
KB_API float     kb_rand_unit();
KB_API float     kb_rand_range(float from, float to);
KB_API Float2    kb_rand_unit_circle();
KB_API Float3    kb_rand_unit_sphere();

#ifdef __cplusplus
}
#endif
