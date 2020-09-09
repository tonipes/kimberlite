#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "kb/core.h"

//#####################################################################################################################
// Handles
//#####################################################################################################################

KB_HANDLE(MusicHandle);
KB_HANDLE(MusicInstanceHandle);

KB_HANDLE(SoundHandle);
KB_HANDLE(SoundInstanceHandle);

//#####################################################################################################################
// Structs
//#####################################################################################################################

typedef struct {
  float param;
} AudioInitInfo;

typedef struct {
  RWops* rwops;
} SoundCreateInfo;

// typedef struct {
//   int param;
// } MusicCreateInfo;

//#####################################################################################################################
// Object management
//#####################################################################################################################

KB_RESOURCE_HASHED_FUNC_DECLS (sound, SoundHandle, SoundCreateInfo)
// KB_RESOURCE_HASHED_FUNC_DECLS (music, MusicHandle, MusicCreateInfo)

KB_RESOURCE_CORE_FUNC_DECLS   (sound, SoundHandle, SoundCreateInfo)
// KB_RESOURCE_CORE_FUNC_DECLS   (music, MusicHandle, MusicCreateInfo)

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

// KB_API void                kb_music_set_track           (MusicHandle handle);
// KB_API void                kb_music_play                ();
// KB_API void                kb_music_stop                ();

#ifdef __cplusplus
}
#endif
