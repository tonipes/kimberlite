#pragma once

#include <kb/api.h>
#include <kb/math.h>

#ifdef __cplusplus
extern "C" {
#endif

KB_API float   perlin_noise(Float2 v);
KB_API float   simplex_noise(Float2 v);
KB_API Float2  cellular_noise(Float2 v);

#ifdef __cplusplus
}
#endif
