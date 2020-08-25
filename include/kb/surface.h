#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "kb/config.h"
#include "kb/math.h"

typedef struct {
  Int2 size;
} SurfaceInitInfo;

KB_API void   kb_surface_init                 (const SurfaceInitInfo info);
KB_API void   kb_surface_set_title            (const char* title);
KB_API void   kb_surface_resize               (Int2 size);
KB_API void   kb_surface_deinit               ();
KB_API Int2   kb_surface_get_size             ();
KB_API float  kb_surface_get_aspect           ();
KB_API void*  kb_surface_get_nwh              ();
KB_API bool   kb_surface_should_close         ();

#ifdef __cplusplus
}
#endif
