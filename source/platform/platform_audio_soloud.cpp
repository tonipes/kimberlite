// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/platform.h>

#include <kb/handle.h>
#include <kb/log.h>
#include <kb/resource.h>
#include <kb/alloc.h>

#include <soloud.h>
#include <soloud_wav.h>

struct kb_sound_ref {
  void* data;
  SoLoud::Wav wav;
};

static SoLoud::Soloud soloud;

KB_RESOURCE_STORAGE_DEF(sound, kb_sound, kb_sound_ref, KB_CONFIG_MAX_SOUNDS);

void kb_platform_sound_construct(kb_sound handle, const kb_sound_create_info info) {
  KB_ASSERT_VALID(handle);

  uint32_t data_size = kb_rwops_size(info.rwops);
  
  sound_ref(handle).data = KB_DEFAULT_ALLOC(data_size);
  
  kb_rwops_read(info.rwops, sound_ref(handle).data, data_size);

  sound_ref(handle).wav = SoLoud::Wav();

  SoLoud::result res = sound_ref(handle).wav.loadMem((unsigned char*) sound_ref(handle).data, data_size, false, false);
  if (res != SoLoud::SO_NO_ERROR) {
    kb_log_error("Sound load error\n");
  }
}

void kb_platform_sound_destruct(kb_sound handle) {
  KB_ASSERT_VALID(handle);

  KB_DEFAULT_FREE(sound_ref(handle).data);
}

KB_API void kb_platform_audio_init(const kb_audio_init_info info) {
  auto result = soloud.init(SoLoud::Soloud::CLIP_ROUNDOFF);

  if (result == SoLoud::SO_NO_ERROR) {
    kb_log_info("Initialized audio");
  } else {
    kb_log_error("Audio initialization failed");
  }
};

KB_API void kb_platform_audio_deinit() {
  soloud.deinit();
};

kb_sound_inst kb_platform_sound_play(kb_sound handle) {
  KB_ASSERT_VALID(handle);
  return KB_HANDLE_FROM_ARRAY(soloud.play(sound_ref(handle).wav));
}

KB_API void kb_platform_sound_stop(kb_sound_inst handle) {
  KB_ASSERT_VALID(handle);
  soloud.stop(kb_to_arr(handle));
}

KB_API void kb_platform_sound_set_volume(kb_sound_inst handle, float volume) {
  KB_ASSERT_VALID(handle);
  soloud.setVolume(kb_to_arr(handle), volume);
}

KB_API void kb_platform_sound_set_speed(kb_sound_inst handle, float speed) {
  KB_ASSERT_VALID(handle);
  soloud.setRelativePlaySpeed(kb_to_arr(handle), speed);
}

KB_API void kb_platform_sound_set_pan(kb_sound_inst handle, float pan) {
  KB_ASSERT_VALID(handle);
  soloud.setPan(kb_to_arr(handle), pan);
}
