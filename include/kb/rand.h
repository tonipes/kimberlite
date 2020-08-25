#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "math.h"

uint32_t  kb_rand_gen();
float     kb_rand_unit();
Float2    kb_rand_unit_circle();
Float3    kb_rand_unit_sphere();

#ifdef __cplusplus
}
#endif
