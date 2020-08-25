#ifdef __cplusplus
extern "C" {
#endif

#include "math.h"

float   perlin_noise(Float2 v);
float   simplex_noise(Float2 v);
Float2  cellular_noise(Float2 v);

#ifdef __cplusplus
}
#endif
