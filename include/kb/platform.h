// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/core.h>
#include <kb/alloc.h>
#include <kb/rwops.h>
#include <kb/math.h>
#include <kb/renderer.h>
#include <kb/input.h>
#include <kb/audio.h>

#ifdef __cplusplus
extern "C" {
#endif

KB_API void           kb_platform_pump_events                         ();
KB_API bool           kb_platform_should_close                        ();
KB_API kb_rwops*      kb_platform_open_file_rwops                     (const char* path);
KB_API void           kb_platform_close_file_rwops                    (kb_rwops* rwops);

KB_API void           kb_platform_graphics_init                       (const kb_graphics_init_info info);
KB_API void           kb_platform_graphics_deinit                     ();
KB_API void           kb_platform_graphics_frame                      ();
KB_API Int2           kb_platform_graphics_get_extent                 ();
KB_API void           kb_platform_graphics_wait_device_idle           ();
KB_API void           kb_platform_graphics_submit_calls               (kb_draw_call* calls, uint32_t call_count);
KB_API void*          kb_platform_graphics_transient_alloc            (uint64_t size, uint64_t align);
KB_API void*          kb_platform_graphics_transient_at               (uint64_t offset);
KB_API uint64_t       kb_platform_graphics_transient_offset           (void* ptr);
KB_API uint64_t       kb_platform_graphics_transient_used             ();
KB_API uint64_t       kb_platform_graphics_transient_capacity         ();

KB_API Int2           kb_platform_surface_get_size                    ();
KB_API float          kb_platform_surface_get_aspect                  ();
KB_API void*          kb_platform_surface_get_native_window_handle    ();
KB_API void           kb_platform_surface_create                      (uint32_t w, uint32_t h);
KB_API float          kb_platform_surface_get_scaling                 ();
KB_API void           kb_platform_surface_destroy                     ();

KB_API void           kb_platform_pipeline_construct                  (kb_pipeline handle, const kb_pipeline_create_info* info);
KB_API void           kb_platform_texture_construct                   (kb_texture handle, const kb_texture_create_info* info);
KB_API void           kb_platform_index_buffer_construct              (kb_index_buffer handle, const kb_index_buffer_create_info* info);
KB_API void           kb_platform_vertex_buffer_construct             (kb_vertex_buffer handle, const kb_vertex_buffer_create_info* info);
KB_API void           kb_platform_sound_construct                     (kb_sound handle, const kb_sound_create_info* info);

KB_API void           kb_platform_pipeline_destruct                   (kb_pipeline handle);
KB_API void           kb_platform_texture_destruct                    (kb_texture handle);
KB_API void           kb_platform_index_buffer_destruct               (kb_index_buffer handle);
KB_API void           kb_platform_vertex_buffer_destruct              (kb_vertex_buffer handle);
KB_API void           kb_platform_sound_destruct                      (kb_sound handle);

KB_API void           kb_platform_input_init                          (const kb_input_init_info info);
KB_API void           kb_platform_input_frame                         ();
KB_API void           kb_platform_input_deinit                        ();
KB_API const char*    kb_platform_input_gamepad_name                  (uint32_t index);
KB_API bool           kb_platform_input_gamepad_connected             (uint32_t index);

KB_API void           kb_platform_audio_init                          (const kb_audio_init_info info);
KB_API void           kb_platform_audio_frame                         ();
KB_API void           kb_platform_audio_deinit                        ();

KB_API kb_sound_inst  kb_platform_sound_play                          (kb_sound handle);
KB_API void           kb_platform_sound_stop                          (kb_sound_inst handle);
KB_API void           kb_platform_sound_set_volume                    (kb_sound_inst instance, float volume);
KB_API void           kb_platform_sound_set_speed                     (kb_sound_inst instance, float speed);
KB_API void           kb_platform_sound_set_pan                       (kb_sound_inst instance, float pan);

#ifdef __cplusplus
}
#endif
