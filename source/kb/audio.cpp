// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/audio.h>
#include <kb/platform.h>

#include <kb/core.h>
#include <kb/stream.h>
#include <kb/rand.h>

KB_RESOURCE_ALLOC_FUNC_DEF  (sound,       kb_sound, kb_sound_create_info, KB_CONFIG_MAX_SOUNDS);
KB_RESOURCE_DATA_HASHED_DEF (sound,       kb_sound);
KB_RESOURCE_STORAGE_DEF     (sound_info,  kb_sound, kb_sound_playback_info, KB_CONFIG_MAX_SOUNDS);

bool has_pitch_range(kb_sound handle) {
  return sound_info_ref(handle)->pitch_range.x > 0 && sound_info_ref(handle)->pitch_range.x > 0;
}

bool has_volume_range(kb_sound handle) {
  return sound_info_ref(handle)->volume_range.x > 0 && sound_info_ref(handle)->volume_range.x > 0;
}

KB_API void kb_audio_init(const kb_audio_init_info info) {
  kb_platform_audio_init(info);
}

KB_API void kb_audio_deinit() {
  kb_platform_audio_deinit();
}

KB_API void kb_sound_construct(kb_sound handle, const kb_sound_create_info info) {
  *sound_info_ref(handle) = info.playback;

  kb_platform_audio_sound_construct(handle, info);
  kb_sound_set_initialized(handle, true);
}

KB_API void kb_sound_destruct(kb_sound handle) {
  kb_platform_audio_sound_destruct(handle);
  kb_sound_set_initialized(handle, false);
}

KB_API kb_sound_inst kb_sound_play(kb_sound handle) {
  kb_sound_inst inst = kb_platform_audio_sound_play(handle);
  
  if (has_volume_range(handle)) {
    kb_sound_set_volume(inst, kb_rand_range(NULL, sound_info_ref(handle)->volume_range.x, sound_info_ref(handle)->volume_range.y));
  }
  
  if (has_pitch_range(handle)) {
    kb_sound_set_speed(inst, kb_rand_range(NULL, sound_info_ref(handle)->pitch_range.x, sound_info_ref(handle)->pitch_range.y));
  }

  return inst;
}

KB_API void kb_sound_stop(kb_sound_inst handle) {
  kb_platform_audio_sound_stop(handle);
}

KB_API void kb_sound_set_volume(kb_sound_inst instance, float volume) {
  kb_platform_audio_sound_set_volume(instance, volume);
}

KB_API void kb_sound_set_speed(kb_sound_inst instance, float speed) {
  kb_platform_audio_sound_set_speed(instance, speed);
}

KB_API void kb_sound_set_pan(kb_sound_inst instance, float pan) {
  kb_platform_audio_sound_set_pan(instance, pan);  
}
