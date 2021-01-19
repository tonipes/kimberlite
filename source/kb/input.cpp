// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/foundation.h>

#include <kb/input.h>
#include <kb/platform.h>

kb_input_state state_prev;
kb_input_state state_curr;
kb_input_state state_next;

void kb_input_init(const kb_input_init_info info) {
  kb_platform_input_init(info);
}

void kb_input_frame() {
  kb_platform_input_frame();

  kb_memcpy(&state_prev,  &state_curr,  sizeof(kb_input_state));
  kb_memcpy(&state_curr,  &state_next,  sizeof(kb_input_state));

  state_next.wheel = {}; // Reset wheel
}

void kb_input_deinit() {
  kb_platform_input_deinit();
}
