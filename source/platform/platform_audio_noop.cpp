// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/platform.h>

void kb_platform_sound_construct(kb_sound handle, const kb_sound_create_info info) {
}

void kb_platform_sound_destruct(kb_sound handle) {
  KB_ASSERT_VALID(handle);
}

KB_API void kb_platform_audio_init(const kb_audio_init_info info) {

};

KB_API void kb_platform_audio_deinit() {

};

kb_sound_inst kb_platform_sound_play(kb_sound handle) {
  KB_ASSERT_VALID(handle);
  return { 0 };
}

KB_API void kb_platform_sound_stop(kb_sound_inst handle) {

}

KB_API void kb_platform_sound_set_volume(kb_sound_inst handle, float volume) {

}

KB_API void kb_platform_sound_set_speed(kb_sound_inst handle, float speed) {

}

KB_API void kb_platform_sound_set_pan(kb_sound_inst handle, float pan) {

}
