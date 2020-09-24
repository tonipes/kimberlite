#pragma once

#include <kb/config.h>
#include <kb/math.h>
#include <kb/api.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  Int2 size;
} SurfaceInitInfo;

KB_API void           kb_surface_init                       (const SurfaceInitInfo info);
KB_API void           kb_surface_set_title                  (const char* title);
KB_API void           kb_surface_resize                     (Int2 size);
KB_API void           kb_surface_deinit                     ();
KB_API Int2           kb_surface_get_size                   ();
KB_API float          kb_surface_get_aspect                 ();
KB_API void*          kb_surface_get_nwh                    ();
KB_API bool           kb_surface_should_close               ();

KB_API void*          kb_surface_get_window_impl            ();

KB_API uint32_t       kb_surface_get_vulkan_extension_count ();
KB_API const char**   kb_surface_get_vulkan_extensions      ();
KB_API void           kb_surface_create_vulkan_surface      (void* instance, void* surface);

#ifdef __cplusplus
}
#endif
