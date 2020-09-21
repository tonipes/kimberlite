#pragma once

#include <kb/config.h>
#include <kb/math.h>
#include <kb/handle.h>
#include <kb/scancodes.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
  int param;
} InputInitInfo;

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

bool          kb_input_key_down                 (Scancode scancode);
bool          kb_input_key_up                   (Scancode scancode);
bool          kb_input_key_released             (Scancode scancode);
bool          kb_input_key_pressed              (Scancode scancode);

bool          kb_input_mouse_button_down        (MouseButton button);
bool          kb_input_mouse_button_up          (MouseButton button);
bool          kb_input_mouse_button_released    (MouseButton button);
bool          kb_input_mouse_button_pressed     (MouseButton button);

Float2        kb_input_mouse_pos                ();
Float2        kb_input_mouse_delta              ();

Float2        kb_input_mouse_wheel              ();
Float2        kb_input_mouse_wheel_delta        ();

bool          kb_input_gamepad_connected        (GamepadHandle gamepad);
const char*   kb_input_gamepad_name             (GamepadHandle gamepad);

bool          kb_input_gamepad_button_down      (GamepadHandle gamepad, GamepadButton button);
bool          kb_input_gamepad_button_up        (GamepadHandle gamepad, GamepadButton button);
bool          kb_input_gamepad_button_released  (GamepadHandle gamepad, GamepadButton button);
bool          kb_input_gamepad_button_pressed   (GamepadHandle gamepad, GamepadButton button);

Scalar        kb_input_gamepad_axis_value       (GamepadHandle gamepad, GamepadAxis axis);
Scalar        kb_input_gamepad_axis_delta       (GamepadHandle gamepad, GamepadAxis axis);

Float2        kb_input_gamepad_joystick_value   (GamepadHandle gamepad, GamepadJoystick joystick);
Float2        kb_input_gamepad_joystick_delta   (GamepadHandle gamepad, GamepadJoystick joystick);

GamepadHandle kb_input_gamepad_get              (uint32_t index);

void          kb_input_init                     (const InputInitInfo info);
void          kb_input_deinit                   ();
void          kb_input_frame                    ();

#ifdef __cplusplus
}
#endif
