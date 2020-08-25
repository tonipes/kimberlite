#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "kb/math.h"
#include "kb/handle.h"

#include "scancodes.h"

#include "kb/config.h"

typedef enum {
  GamepadAxisInvalid,
  GamepadAxisLeftX,
  GamepadAxisLeftY,
  GamepadAxisRightX,
  GamepadAxisRightY,
  GamepadAxisTriggerLeft,
  GamepadAxisTriggerRight,
  GamepadAxisCount,
} GamepadAxis;

typedef enum {
  GamepadButtonInvalid,
  GamepadButtonA,
  GamepadButtonB,
  GamepadButtonX,
  GamepadButtonY,
  GamepadButtonBack,
  GamepadButtonGuide,
  GamepadButtonStart,
  GamepadButtonLeftStick,
  GamepadButtonRightStick,
  GamepadButtonLeftShoulder,
  GamepadButtonRightShoulder,
  GamepadButtonDpadUp,
  GamepadButtonDpadDown,
  GamepadButtonDpadLeft,
  GamepadButtonDpadRight,
  GamepadButtonCount,
} GamepadButton;

typedef enum {
  GamepadJoystickLeft,
  GamepadJoystickRight,
  GamepadJoystickCount,
} GamepadJoystick;


typedef struct {
  int16_t axis[GamepadAxisCount];
  uint8_t button[GamepadButtonCount];
} GamepadState;

typedef enum {
  MouseButtonUnknown,
  MouseButtonLeft,
  MouseButtonRight,
  MouseButtonMiddle,
  MouseButtonCount,
} MouseButton;

KB_HANDLE(GamepadHandle);

bool input_key_down                (Scancode scancode);
bool input_key_up                  (Scancode scancode);

bool input_key_released            (Scancode scancode);
bool input_key_pressed             (Scancode scancode);

bool input_mouse_button_down       (MouseButton button);
bool input_mouse_button_up         (MouseButton button);
bool input_mouse_button_released   (MouseButton button);
bool input_mouse_button_pressed    (MouseButton button);

Float2 input_mouse_pos();
Float2 input_mouse_delta();

Float2 input_mouse_wheel();
Float2 input_mouse_wheel_delta();

bool input_gamepad_connected        (GamepadHandle gamepad);
const char* input_gamepad_name      (GamepadHandle gamepad);

bool input_gamepad_button_down      (GamepadHandle gamepad, GamepadButton button);
bool input_gamepad_button_up        (GamepadHandle gamepad, GamepadButton button);
bool input_gamepad_button_released  (GamepadHandle gamepad, GamepadButton button);
bool input_gamepad_button_pressed   (GamepadHandle gamepad, GamepadButton button);

Scalar input_gamepad_axis_value     (GamepadHandle gamepad, GamepadAxis axis);
Scalar input_gamepad_axis_delta     (GamepadHandle gamepad, GamepadAxis axis);

Float2 input_gamepad_joystick_value (GamepadHandle gamepad, GamepadJoystick joystick);
Float2 input_gamepad_joystick_delta (GamepadHandle gamepad, GamepadJoystick joystick);

GamepadHandle get_gamepad(uint32_t index);

void haptic_test(GamepadHandle handle);

void input_init();
void input_update();

#ifdef __cplusplus
}
#endif
