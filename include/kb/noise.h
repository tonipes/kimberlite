#pragma once

#include <kb/api.h>
#include <kb/math.h>

#ifdef __cplusplus
extern "C" {
#endif

KB_API float   kb_noise_perlin    (Float2 v);
KB_API float   kb_noise_simplex   (Float2 v);
KB_API Float2  kb_noise_cellular  (Float2 v);

#ifdef __cplusplus
}
#endif
