// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/platform.h>

KB_API void kb_platform_input_init(const kb_input_init_info info) {

}

KB_API void kb_platform_input_frame() {

}

KB_API void kb_platform_input_deinit() {

}

KB_API const char* kb_input_gamepad_name(uint32_t gamepad) {
  return "Unknown";
}

KB_API bool kb_input_gamepad_connected(uint32_t gamepad) {
  return false;
}
