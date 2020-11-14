// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/platform.h>

#include <kb/core.h>
#include <kb/log.h>
#include <kb/handle.h>
#include <kb/freelist.h>
#include <kb/table.h>
#include <kb/crt.h>
#include <kb/rwops.h>

#include <SDL2/SDL.h>

kb_freelist           gamepad_freelist;
kb_table              gamepad_table;
SDL_GameController*   gamepad_ptrs[KB_CONFIG_MAX_GAMEPADS];

bool kb_input_gamepad_connected(uint32_t gamepad) {
  return gamepad_ptrs[gamepad] != NULL;
}

const char* kb_input_gamepad_name(uint32_t gamepad) {
  if (!kb_input_gamepad_connected(gamepad)) return nullptr;
  return SDL_GameControllerName(gamepad_ptrs[gamepad]);
}

void gamepad_add(int32_t i) {
  uint32_t slot = kb_freelist_take(&gamepad_freelist);
  gamepad_ptrs[slot] = SDL_GameControllerOpen(i);
  
  SDL_JoystickID id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gamepad_ptrs[slot]));
  kb_table_insert(&gamepad_table, id, slot);

  kb_log_debug("Gamepad added ({}) ({})", slot, id);
}

void gamepad_remove(int32_t i) {
  uint32_t slot = kb_table_get(&gamepad_table, i);

  if (slot != UINT32_MAX && gamepad_ptrs[slot] != nullptr) {
    SDL_GameControllerClose(gamepad_ptrs[slot]);    
    gamepad_ptrs[slot] = nullptr;
    kb_freelist_return(&gamepad_freelist, slot);
    kb_table_remove(&gamepad_table, i);
    
    kb_log_debug("Gamepad removed ({}) ({})", slot, i);
  }
}

int event_watch(void* userdata, SDL_Event* event) {
  if (event->type == SDL_MOUSEWHEEL) {
    state_next.wheel.x = event->wheel.x;
    state_next.wheel.y = event->wheel.y;
  }

  if (event->type == SDL_MOUSEMOTION) {
    state_next.mouse_pos.x = event->motion.x;
    state_next.mouse_pos.y = event->motion.y;
  }
  
  if (event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
    state_next.mouse[event->button.button] = event->button.state;
  }
  
  if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
    if (!event->key.repeat) {
      state_next.keyboard[event->key.keysym.scancode] = event->key.state;
    }
  }

  if (event->type == SDL_CONTROLLERAXISMOTION) {
    SDL_JoystickID id = event->jaxis.which;
    
    uint32_t slot = kb_table_get(&gamepad_table, id);
    if (slot != UINT32_MAX) {
      state_next.gamepad[slot].axis[event->jaxis.axis + 1] = event->jaxis.value; // HACK: offset +1
    }
  }

  if (event->type == SDL_CONTROLLERBUTTONDOWN | event->type == SDL_CONTROLLERBUTTONUP) {
    SDL_JoystickID id = event->jbutton.which;
    uint32_t slot = kb_table_get(&gamepad_table, id);
    if (slot != UINT32_MAX) {
      state_next.gamepad[slot].button[event->jbutton.button + 1] = event->jbutton.state; // HACK: offset +1
    }
  }
  
  if (event->type == SDL_JOYDEVICEADDED) {
    if (SDL_IsGameController(event->jdevice.which)) {
      kb_log_debug("Add joy {}", event->jdevice.which);
      gamepad_add(event->jdevice.which);
    }
  }

  if (event->type == SDL_JOYDEVICEREMOVED) {
    kb_log_debug("Remove joy {}", event->jdevice.which);
    gamepad_remove(event->jdevice.which);
  }

  return 1;
}

void kb_platform_input_init(const kb_input_init_info info) {
  kb_freelist_create  (&gamepad_freelist, KB_CONFIG_MAX_GAMEPADS);
  kb_table_create     (&gamepad_table,    KB_CONFIG_MAX_GAMEPADS);

  SDL_AddEventWatch(event_watch, nullptr);
  
  if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER)) {
    kb_log_error("Failed to init input gamecontrollers: {}", SDL_GetError());
  }
}

void kb_platform_input_frame() {

}

void kb_platform_input_deinit() {

}
