// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/core.h>
#include <kb/alloc.h>
#include <kb/stream.h>
#include <kb/math.h>
#include <kb/graphics.h>
#include <kb/input.h>
#include <kb/audio.h>

#ifdef __cplusplus
extern "C" {
#endif

KB_API void           kb_platform_pump_events                         ();
KB_API bool           kb_platform_should_close                        ();

KB_API kb_stream*     kb_platform_rwops_file_open                     (const char* path);
KB_API void           kb_platform_rwops_file_close                    (kb_stream* rwops);

KB_API void           kb_platform_graphics_init                       (const kb_graphics_init_info info);
KB_API void           kb_platform_graphics_deinit                     ();
KB_API void           kb_platform_graphics_frame                      ();
KB_API Int2           kb_platform_graphics_get_surface_extent         ();
KB_API float          kb_platform_graphics_get_surface_scale          ();
KB_API void           kb_platform_graphics_wait_device_idle           ();
KB_API void           kb_platform_graphics_submit_calls               (uint32_t pass, kb_graphics_call* calls, uint32_t call_count);
KB_API void*          kb_platform_graphics_transient_alloc            (uint64_t size, uint64_t align);
KB_API void*          kb_platform_graphics_transient_at               (uint64_t offset);
KB_API uint64_t       kb_platform_graphics_transient_offset           (void* ptr);
KB_API uint64_t       kb_platform_graphics_transient_used             ();
KB_API uint64_t       kb_platform_graphics_transient_capacity         ();
KB_API void*          kb_platform_graphics_buffer_mapped              (kb_buffer buffer);
KB_API Int2           kb_platform_graphics_surface_get_size           ();
KB_API void           kb_platform_graphics_buffer_construct           (kb_buffer handle, const kb_buffer_create_info info);
KB_API void           kb_platform_graphics_pipeline_construct         (kb_pipeline handle, const kb_pipeline_create_info info);
KB_API void           kb_platform_graphics_texture_construct          (kb_texture handle, const kb_texture_create_info info);
KB_API void           kb_platform_graphics_buffer_destruct            (kb_buffer handle);
KB_API void           kb_platform_graphics_pipeline_destruct          (kb_pipeline handle);
KB_API void           kb_platform_graphics_texture_destruct           (kb_texture handle);

KB_API void           kb_platform_input_init                          (const kb_input_init_info info);
KB_API void           kb_platform_input_frame                         ();
KB_API void           kb_platform_input_deinit                        ();
KB_API const char*    kb_platform_input_gamepad_name                  (uint32_t index);
KB_API bool           kb_platform_input_gamepad_connected             (uint32_t index);

KB_API void           kb_platform_audio_sound_construct               (kb_sound handle, const kb_sound_create_info info);
KB_API void           kb_platform_audio_sound_destruct                (kb_sound handle);
KB_API void           kb_platform_audio_init                          (const kb_audio_init_info info);
KB_API void           kb_platform_audio_frame                         ();
KB_API void           kb_platform_audio_deinit                        ();
KB_API kb_sound_inst  kb_platform_audio_sound_play                    (kb_sound handle);
KB_API void           kb_platform_audio_sound_stop                    (kb_sound_inst handle);
KB_API void           kb_platform_audio_sound_set_volume              (kb_sound_inst instance, float volume);
KB_API void           kb_platform_audio_sound_set_speed               (kb_sound_inst instance, float speed);
KB_API void           kb_platform_audio_sound_set_pan                 (kb_sound_inst instance, float pan);

#ifdef __cplusplus
}
#endif
