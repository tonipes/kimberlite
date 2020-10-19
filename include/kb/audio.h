#pragma once

#include <kb/core.h>
#include <kb/handle.h>
#include <kb/resource.h>
#include <kb/math.h>
#include <kb/rwops.h>

KB_HANDLE(kb_sound);
KB_HANDLE(kb_sound_inst);

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kb_sound_playback_info {
  Float2                  pitch_range;
  Float2                  volume_range;
} kb_sound_playback_info;

typedef struct kb_audio_init_info {
  int                     unused;
} kb_audio_init_info;

typedef struct kb_sound_create_info {
  kb_rwops*               rwops;
  kb_sound_playback_info  playback;
} kb_sound_create_info;

KB_RESOURCE_HASHED_FUNC_DECLS (sound, kb_sound, kb_sound_create_info);
KB_RESOURCE_ALLOC_FUNC_DECLS  (sound, kb_sound, kb_sound_create_info);

KB_API void           kb_audio_init         (const kb_audio_init_info info);
KB_API void           kb_audio_deinit       ();
KB_API kb_sound_inst  kb_sound_play         (kb_sound handle);
KB_API void           kb_sound_stop         (kb_sound_inst handle);
KB_API void           kb_sound_set_volume   (kb_sound_inst instance, float volume);
KB_API void           kb_sound_set_speed    (kb_sound_inst instance, float speed);
KB_API void           kb_sound_set_pan      (kb_sound_inst instance, float pan);

#ifdef __cplusplus
}
#endif
