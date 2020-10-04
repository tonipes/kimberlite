#pragma once

//#include <kb/config.h>
#include <kb/math.h>
#include <kb/handle.h>
#include <kb/scancodes.h>

KB_HANDLE(kb_gamepad);

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int param;
} kb_input_init_info;

typedef enum {
  KB_GAMEPAD_AXIS_INVALID,
  KB_GAMEPAD_AXIS_LEFTX,
  KB_GAMEPAD_AXIS_LEFTY,
  KB_GAMEPAD_AXIS_RIGHTX,
  KB_GAMEPAD_AXIS_RIGHTY,
  KB_GAMEPAD_AXIS_TRIGL,
  KB_GAMEPAD_AXIS_TRIGR,
  KB_GAMEPAD_AXIS_COUNT,
} kb_gamepad_axis;

typedef enum {
  KB_GAMEPAD_BUTTON_INVALID,
  KB_GAMEPAD_BUTTON_A,
  KB_GAMEPAD_BUTTON_B,
  KB_GAMEPAD_BUTTON_X,
  KB_GAMEPAD_BUTTON_Y,
  KB_GAMEPAD_BUTTON_BACK,
  KB_GAMEPAD_BUTTON_GUIDE,
  KB_GAMEPAD_BUTTON_START,
  KB_GAMEPAD_BUTTON_LEFTSTICK,
  KB_GAMEPAD_BUTTON_RIGHTSTICK,
  KB_GAMEPAD_BUTTON_LEFTSHOULDER,
  KB_GAMEPAD_BUTTON_RIGHTSHOULDER,
  KB_GAMEPAD_BUTTON_DPADUP,
  KB_GAMEPAD_BUTTON_DPADDOWN,
  KB_GAMEPAD_BUTTON_DPADLEFT,
  KB_GAMEPAD_BUTTON_DPADRIGHT,
  KB_GAMEPAD_BUTTON_COUNT,
} kb_gamepad_button;

typedef enum {
  KB_GAMEPAD_JOYSTICK_LEFT,
  KB_GAMEPAD_JOYSTICK_RIGHT,
  KB_GAMEPAD_JOYSTICK_COUNT,
} kb_gamepad_joystick;

typedef enum {
  KB_MOUSE_BUTTON_UNKNOWN,
  KB_MOUSE_BUTTON_LEFT,
  KB_MOUSE_BUTTON_RIGHT,
  KB_MOUSE_BUTTON_MIDDLE,
  KB_MOUSE_BUTTON_COUNT,
} kb_mouse_button;

bool          kb_input_key_down                 (Scancode scancode);
bool          kb_input_key_up                   (Scancode scancode);
bool          kb_input_key_released             (Scancode scancode);
bool          kb_input_key_pressed              (Scancode scancode);

bool          kb_input_mouse_button_down        (kb_mouse_button button);
bool          kb_input_mouse_button_up          (kb_mouse_button button);
bool          kb_input_mouse_button_released    (kb_mouse_button button);
bool          kb_input_mouse_button_pressed     (kb_mouse_button button);

Float2        kb_input_mouse_pos                ();
Float2        kb_input_mouse_delta              ();

Float2        kb_input_mouse_wheel              ();
Float2        kb_input_mouse_wheel_delta        ();

bool          kb_input_gamepad_connected        (kb_gamepad gamepad);
const char*   kb_input_gamepad_name             (kb_gamepad gamepad);

bool          kb_input_gamepad_button_down      (kb_gamepad gamepad, kb_gamepad_button button);
bool          kb_input_gamepad_button_up        (kb_gamepad gamepad, kb_gamepad_button button);
bool          kb_input_gamepad_button_released  (kb_gamepad gamepad, kb_gamepad_button button);
bool          kb_input_gamepad_button_pressed   (kb_gamepad gamepad, kb_gamepad_button button);

Real32        kb_input_gamepad_axis_value       (kb_gamepad gamepad, kb_gamepad_axis axis);
Real32        kb_input_gamepad_axis_delta       (kb_gamepad gamepad, kb_gamepad_axis axis);

Float2        kb_input_gamepad_joystick_value   (kb_gamepad gamepad, kb_gamepad_joystick joystick);
Float2        kb_input_gamepad_joystick_delta   (kb_gamepad gamepad, kb_gamepad_joystick joystick);

kb_gamepad    kb_input_gamepad_get              (uint32_t index);

void          kb_input_init                     (const kb_input_init_info info);
void          kb_input_deinit                   ();
void          kb_input_frame                    ();

#ifdef __cplusplus
}
#endif
