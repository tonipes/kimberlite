#pragma once

#include <kb/core.h>

#ifdef __cplusplus
extern "C" {
#endif

//#####################################################################################################################
// Handles
//#####################################################################################################################

KB_HANDLE(SoundHandle);
KB_HANDLE(SoundInstanceHandle);

//#####################################################################################################################
// Structs
//#####################################################################################################################

typedef struct {
  Float2 pitch_range;
  Float2 volume_range;
} SoundPlaybackInfo;

typedef struct {
  float param;
} AudioInitInfo;

typedef struct {
  RWops* rwops;
  SoundPlaybackInfo playback;
} SoundCreateInfo;

//#####################################################################################################################
// Object management
//#####################################################################################################################

KB_RESOURCE_HASHED_FUNC_DECLS (sound, SoundHandle, SoundCreateInfo)
KB_RESOURCE_CORE_FUNC_DECLS   (sound, SoundHandle, SoundCreateInfo)

//#####################################################################################################################
// API functions
//#####################################################################################################################

KB_API void                kb_audio_init                (const AudioInitInfo info);
KB_API void                kb_audio_deinit              ();

KB_API SoundInstanceHandle kb_sound_play                (SoundHandle handle);
KB_API void                kb_sound_stop                (SoundInstanceHandle handle);
KB_API void                kb_sound_set_volume          (SoundInstanceHandle instance, float volume);
KB_API void                kb_sound_set_looping         (SoundInstanceHandle instance, bool looping);
KB_API void                kb_sound_set_speed           (SoundInstanceHandle instance, float speed);
KB_API void                kb_sound_set_pan             (SoundInstanceHandle instance, float pan);

KB_API void                kb_sound_wait                (SoundInstanceHandle instance);


#ifdef __cplusplus
}
#endif
