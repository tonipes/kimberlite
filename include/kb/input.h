// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/math.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kb_scancode {
  KB_KEY_INVALID            = 0,
  KB_KEY_MOD_CTRL_LEFT      = 0x01u,
  KB_KEY_MOD_SHIFT_LEFT     = 0x02u,
  KB_KEY_MOD_ALT_LEFT       = 0x04u,
  KB_KEY_MOD_META_LEFT      = 0x08u,
  KB_KEY_MOD_CTRL_RIGHT     = 0x10u,
  KB_KEY_MOD_SHIFT_RIGHT    = 0x20u,
  KB_KEY_MOD_ALT_RIGHT      = 0x40u,
  KB_KEY_MOD_META_RIGHT     = 0x80u,
  KB_KEY_NONE               = 0x00u, // No key pressed
  KB_KEY_ERROVF             = 0x01u, // Keyboard Error Roll Over
  KB_KEY_A                  = 0x04u, // Keyboard a and A
  KB_KEY_B                  = 0x05u, // Keyboard b and B
  KB_KEY_C                  = 0x06u, // Keyboard c and C
  KB_KEY_D                  = 0x07u, // Keyboard d and D
  KB_KEY_E                  = 0x08u, // Keyboard e and E
  KB_KEY_F                  = 0x09u, // Keyboard f and F
  KB_KEY_G                  = 0x0au, // Keyboard g and G
  KB_KEY_H                  = 0x0bu, // Keyboard h and H
  KB_KEY_I                  = 0x0cu, // Keyboard i and I
  KB_KEY_J                  = 0x0du, // Keyboard j and J
  KB_KEY_K                  = 0x0eu, // Keyboard k and K
  KB_KEY_L                  = 0x0fu, // Keyboard l and L
  KB_KEY_M                  = 0x10u, // Keyboard m and M
  KB_KEY_N                  = 0x11u, // Keyboard n and N
  KB_KEY_O                  = 0x12u, // Keyboard o and O
  KB_KEY_P                  = 0x13u, // Keyboard p and P
  KB_KEY_Q                  = 0x14u, // Keyboard q and Q
  KB_KEY_R                  = 0x15u, // Keyboard r and R
  KB_KEY_S                  = 0x16u, // Keyboard s and S
  KB_KEY_T                  = 0x17u, // Keyboard t and T
  KB_KEY_U                  = 0x18u, // Keyboard u and U
  KB_KEY_V                  = 0x19u, // Keyboard v and V
  KB_KEY_W                  = 0x1au, // Keyboard w and W
  KB_KEY_X                  = 0x1bu, // Keyboard x and X
  KB_KEY_Y                  = 0x1cu, // Keyboard y and Y
  KB_KEY_Z                  = 0x1du, // Keyboard z and Z
  KB_KEY_1                  = 0x1eu, // Keyboard 1 and !
  KB_KEY_2                  = 0x1fu, // Keyboard 2 and @
  KB_KEY_3                  = 0x20u, // Keyboard 3 and #
  KB_KEY_4                  = 0x21u, // Keyboard 4 and $
  KB_KEY_5                  = 0x22u, // Keyboard 5 and %
  KB_KEY_6                  = 0x23u, // Keyboard 6 and ^
  KB_KEY_7                  = 0x24u, // Keyboard 7 and &
  KB_KEY_8                  = 0x25u, // Keyboard 8 and *
  KB_KEY_9                  = 0x26u, // Keyboard 9 and (
  KB_KEY_0                  = 0x27u, // Keyboard 0 and )
  KB_KEY_ENTER              = 0x28u, // Keyboard Return (ENTER)
  KB_KEY_ESC                = 0x29u, // Keyboard ESCAPE
  KB_KEY_BACKSPACE          = 0x2au, // Keyboard DELETE (Backspace)
  KB_KEY_TAB                = 0x2bu, // Keyboard Tab
  KB_KEY_SPACE              = 0x2cu, // Keyboard Spacebar
  KB_KEY_MINUS              = 0x2du, // Keyboard - and _
  KB_KEY_EQUAL              = 0x2eu, // Keyboard = and +
  KB_KEY_BRACE_OPEN         = 0x2fu, // Keyboard [ and {
  KB_KEY_BRACE_CLOSE        = 0x30u, // Keyboard ] and }
  KB_KEY_BACKSLASH          = 0x31u, // Keyboard \ and |
  KB_KEY_HASH               = 0x32u, // Keyboard Non-US # and ~
  KB_KEY_SEMICOLON          = 0x33u, // Keyboard ; and :
  KB_KEY_APOSTROPHE         = 0x34u, // Keyboard ' and "
  KB_KEY_GRAVE              = 0x35u, // Keyboard ` and ~
  KB_KEY_COMMA              = 0x36u, // Keyboard , and <
  KB_KEY_DOT                = 0x37u, // Keyboard . and >
  KB_KEY_SLASH              = 0x38u, // Keyboard / and ?
  KB_KEY_CAPSLOCK           = 0x39u, // Keyboard Caps Lock
  KB_KEY_F1                 = 0x3au, // Keyboard F1
  KB_KEY_F2                 = 0x3bu, // Keyboard F2
  KB_KEY_F3                 = 0x3cu, // Keyboard F3
  KB_KEY_F4                 = 0x3du, // Keyboard F4
  KB_KEY_F5                 = 0x3eu, // Keyboard F5
  KB_KEY_F6                 = 0x3fu, // Keyboard F6
  KB_KEY_F7                 = 0x40u, // Keyboard F7
  KB_KEY_F8                 = 0x41u, // Keyboard F8
  KB_KEY_F9                 = 0x42u, // Keyboard F9
  KB_KEY_F10                = 0x43u, // Keyboard F10
  KB_KEY_F11                = 0x44u, // Keyboard F11
  KB_KEY_F12                = 0x45u, // Keyboard F12
  KB_KEY_SYSRQ              = 0x46u, // Keyboard Print Screen
  KB_KEY_SCROLK             = 0x47u, // Keyboard Scroll Lock
  KB_KEY_PAUSE              = 0x48u, // Keyboard Pause
  KB_KEY_INSERT             = 0x49u, // Keyboard Insert
  KB_KEY_HOME               = 0x4au, // Keyboard Home
  KB_KEY_PAGEUP             = 0x4bu, // Keyboard Page Up
  KB_KEY_DELETE             = 0x4cu, // Keyboard Delete Forward
  KB_KEY_END                = 0x4du, // Keyboard End
  KB_KEY_PAGEDOWN           = 0x4eu, // Keyboard Page Down
  KB_KEY_RIGHT              = 0x4fu, // Keyboard Right Arrow
  KB_KEY_LEFT               = 0x50u, // Keyboard Left Arrow
  KB_KEY_DOWN               = 0x51u, // Keyboard Down Arrow
  KB_KEY_UP                 = 0x52u, // Keyboard Up Arrow
  KB_KEY_NUMLK              = 0x53u, // Keyboard Num Lock and Clear
  KB_KEY_KP_SLASH           = 0x54u, // Keypad /
  KB_KEY_KP_ASTERISK        = 0x55u, // Keypad *
  KB_KEY_KP_MINUS           = 0x56u, // Keypad -
  KB_KEY_KP_PLUS            = 0x57u, // Keypad +
  KB_KEY_KP_ENTER           = 0x58u, // Keypad ENTER
  KB_KEY_KP_1               = 0x59u, // Keypad 1 and End
  KB_KEY_KP_2               = 0x5au, // Keypad 2 and Down Arrow
  KB_KEY_KP_3               = 0x5bu, // Keypad 3 and PageDn
  KB_KEY_KP_4               = 0x5cu, // Keypad 4 and Left Arrow
  KB_KEY_KP_5               = 0x5du, // Keypad 5
  KB_KEY_KP_6               = 0x5eu, // Keypad 6 and Right Arrow
  KB_KEY_KP_7               = 0x5fu, // Keypad 7 and Home
  KB_KEY_KP_8               = 0x60u, // Keypad 8 and Up Arrow
  KB_KEY_KP_9               = 0x61u, // Keypad 9 and Page Up
  KB_KEY_KP_0               = 0x62u, // Keypad 0 and Insert
  KB_KEY_KP_DOT             = 0x63u, // Keypad . and Delete
  // KB_KEY_102ND              = 0x64u, // Keyboard Non-US \ and |
  KB_KEY_COMPOSE            = 0x65u, // Keyboard Application
  KB_KEY_POWER              = 0x66u, // Keyboard Power
  KB_KEY_KP_EQUAL           = 0x67u, // Keypad =
  KB_KEY_F13                = 0x68u, // Keyboard F13
  KB_KEY_F14                = 0x69u, // Keyboard F14
  KB_KEY_F15                = 0x6au, // Keyboard F15
  KB_KEY_F16                = 0x6bu, // Keyboard F16
  KB_KEY_F17                = 0x6cu, // Keyboard F17
  KB_KEY_F18                = 0x6du, // Keyboard F18
  KB_KEY_F19                = 0x6eu, // Keyboard F19
  KB_KEY_F20                = 0x6fu, // Keyboard F20
  KB_KEY_F21                = 0x70u, // Keyboard F21
  KB_KEY_F22                = 0x71u, // Keyboard F22
  KB_KEY_F23                = 0x72u, // Keyboard F23
  KB_KEY_F24                = 0x73u, // Keyboard F24
  KB_KEY_OPEN               = 0x74u, // Keyboard Execute
  KB_KEY_HELP               = 0x75u, // Keyboard Help
  KB_KEY_PROPS              = 0x76u, // Keyboard Menu
  KB_KEY_FRONT              = 0x77u, // Keyboard Select
  KB_KEY_STOP               = 0x78u, // Keyboard Stop
  KB_KEY_AGAIN              = 0x79u, // Keyboard Again
  KB_KEY_UNDO               = 0x7au, // Keyboard Undo
  KB_KEY_CUT                = 0x7bu, // Keyboard Cut
  KB_KEY_COPY               = 0x7cu, // Keyboard Copy
  KB_KEY_PASTE              = 0x7du, // Keyboard Paste
  KB_KEY_FIND               = 0x7eu, // Keyboard Find
  KB_KEY_MUTE               = 0x7fu, // Keyboard Mute
  KB_KEY_VOL_UP             = 0x80u, // Keyboard Volume Up
  KB_KEY_VOL_DOWN           = 0x81u, // Keyboard Volume Down
  KB_KEY_KP_COMMA           = 0x85u, // Keypad Comma
  KB_KEY_RO                 = 0x87u, // Keyboard International1
  KB_KEY_KATAKANAHIRAGANA   = 0x88u, // Keyboard International2
  KB_KEY_YEN                = 0x89u, // Keyboard International3
  KB_KEY_HENKAN             = 0x8au, // Keyboard International4
  KB_KEY_MUHENKAN           = 0x8bu, // Keyboard International5
  KB_KEY_KP_JPCOMMA         = 0x8cu, // Keyboard International6
  KB_KEY_HANGEUL            = 0x90u, // Keyboard LANG1
  KB_KEY_HANJA              = 0x91u, // Keyboard LANG2
  KB_KEY_KATAKANA           = 0x92u, // Keyboard LANG3
  KB_KEY_HIRAGANA           = 0x93u, // Keyboard LANG4
  KB_KEY_ZENKAKUHANKAKU     = 0x94u, // Keyboard LANG5
  KB_KEY_KP_PAREN_LEFT      = 0xb6u, // Keypad (
  KB_KEY_KP_PAREN_RIGHT     = 0xb7u, // Keypad )
  KB_KEY_CTRL_LEFT          = 0xe0u, // Keyboard Left Control
  KB_KEY_SHIFT_LEFT         = 0xe1u, // Keyboard Left Shift
  KB_KEY_ALT_LEFT           = 0xe2u, // Keyboard Left Alt
  KB_KEY_META_LEFT          = 0xe3u, // Keyboard Left GUI
  KB_KEY_CTRL_RIGHT         = 0xe4u, // Keyboard Right Control
  KB_KEY_SHIFT_RIGHT        = 0xe5u, // Keyboard Right Shift
  KB_KEY_ALT_RIGHT          = 0xe6u, // Keyboard Right Alt
  KB_KEY_META_RIGHT         = 0xe7u, // Keyboard Right GUI
  KB_KEY_MEDIA_PLAY         = 0xe8u, 
  KB_KEY_MEDIA_STOPCD       = 0xe9u,
  KB_KEY_MEDIA_PREVIOUS     = 0xeau,
  KB_KEY_MEDIA_NEXT         = 0xebu,
  KB_KEY_MEDIA_EJECT        = 0xecu,
  KB_KEY_MEDIA_VOL_UP       = 0xedu,
  KB_KEY_MEDIA_VOL_DOWN     = 0xeeu,
  KB_KEY_MEDIA_MUTE         = 0xefu,
  KB_KEY_MEDIA_WWW          = 0xf0u,
  KB_KEY_MEDIA_BACK         = 0xf1u,
  KB_KEY_MEDIA_FORWARD      = 0xf2u,
  KB_KEY_MEDIA_STOP         = 0xf3u,
  KB_KEY_MEDIA_FIND         = 0xf4u,
  KB_KEY_MEDIA_SCROLL_UP    = 0xf5u,
  KB_KEY_MEDIA_SCROLL_DOWN  = 0xf6u,
  KB_KEY_MEDIA_EDIT         = 0xf7u,
  KB_KEY_MEDIA_SLEEP        = 0xf8u,
  KB_KEY_MEDIA_COFFEE       = 0xf9u,
  KB_KEY_MEDIA_REFRESH      = 0xfau,
  KB_KEY_MEDIA_CALC         = 0xfbu,
  KB_KEY_COUNT              = 0xffu,
} kb_scancode;

typedef enum kb_gamepad_axis {
  KB_GAMEPAD_AXIS_INVALID   = 0,
  KB_GAMEPAD_AXIS_LEFTX     = 1,
  KB_GAMEPAD_AXIS_LEFTY     = 2,
  KB_GAMEPAD_AXIS_RIGHTX    = 3,
  KB_GAMEPAD_AXIS_RIGHTY    = 4,
  KB_GAMEPAD_AXIS_TRIGL     = 5,
  KB_GAMEPAD_AXIS_TRIGR     = 6,
  KB_GAMEPAD_AXIS_COUNT     = 7,
} kb_gamepad_axis;

typedef enum kb_gamepad_button {
  KB_GAMEPAD_BUTTON_INVALID       = 0,
  KB_GAMEPAD_BUTTON_A             = 1,
  KB_GAMEPAD_BUTTON_B             = 2,
  KB_GAMEPAD_BUTTON_X             = 3,
  KB_GAMEPAD_BUTTON_Y             = 4,
  KB_GAMEPAD_BUTTON_BACK          = 5,
  KB_GAMEPAD_BUTTON_GUIDE         = 6,
  KB_GAMEPAD_BUTTON_START         = 7,
  KB_GAMEPAD_BUTTON_LEFTSTICK     = 8,
  KB_GAMEPAD_BUTTON_RIGHTSTICK    = 9,
  KB_GAMEPAD_BUTTON_LEFTSHOULDER  = 10,
  KB_GAMEPAD_BUTTON_RIGHTSHOULDER = 11,
  KB_GAMEPAD_BUTTON_DPADUP        = 12,
  KB_GAMEPAD_BUTTON_DPADDOWN      = 13,
  KB_GAMEPAD_BUTTON_DPADLEFT      = 14,
  KB_GAMEPAD_BUTTON_DPADRIGHT     = 15,
  KB_GAMEPAD_BUTTON_COUNT         = 16,
} kb_gamepad_button;

typedef enum kb_gamepad_joystick {
  KB_GAMEPAD_JOYSTICK_INVALID = 0,
  KB_GAMEPAD_JOYSTICK_LEFT    = 1,
  KB_GAMEPAD_JOYSTICK_RIGHT   = 2,
  KB_GAMEPAD_JOYSTICK_COUNT   = 3,
} kb_gamepad_joystick;

typedef enum kb_mouse_button {
  KB_MOUSE_BUTTON_INVALID = 0,
  KB_MOUSE_BUTTON_LEFT    = 1,
  KB_MOUSE_BUTTON_MIDDLE  = 2,
  KB_MOUSE_BUTTON_RIGHT   = 3,
  KB_MOUSE_BUTTON_COUNT   = 4,
} kb_mouse_button;

typedef struct kb_input_init_info {
  int               unused;
} kb_input_init_info;

typedef struct kb_gamepad_state {
  float             axis      [KB_GAMEPAD_AXIS_COUNT];
  uint8_t           button    [KB_GAMEPAD_BUTTON_COUNT];
} kb_gamepad_state;

typedef struct kb_input_state {
  uint8_t           keyboard  [KB_KEY_COUNT];
  uint8_t           mouse     [KB_MOUSE_BUTTON_COUNT];
  kb_gamepad_state  gamepad   [KB_CONFIG_MAX_GAMEPADS];
  Float2            mouse_pos;
  Float2            wheel;
} kb_input_state;

typedef struct kb_gamepad_joystick_axis {
  kb_gamepad_axis x, y;
} kb_gamepad_joystick_axis;

static kb_gamepad_joystick_axis joystick_axis[4] = {
  { KB_GAMEPAD_AXIS_INVALID,  KB_GAMEPAD_AXIS_INVALID },
  { KB_GAMEPAD_AXIS_LEFTX,    KB_GAMEPAD_AXIS_LEFTY },
  { KB_GAMEPAD_AXIS_RIGHTX,   KB_GAMEPAD_AXIS_RIGHTY },
  { KB_GAMEPAD_AXIS_INVALID,  KB_GAMEPAD_AXIS_INVALID }
};

extern kb_input_state state_prev;
extern kb_input_state state_curr;
extern kb_input_state state_next;

KB_API_INLINE   kb_input_state* kb_input_get_state_prev               (void) { return &state_prev; }
KB_API_INLINE   kb_input_state* kb_input_get_state_curr               (void) { return &state_curr; }
KB_API_INLINE   kb_input_state* kb_input_get_state_next               (void) { return &state_next; }

KB_API          void          kb_input_init                           (const kb_input_init_info info);
KB_API          void          kb_input_deinit                         (void);
KB_API          void          kb_input_frame                          (void);

KB_API_INLINE   bool          kb_input_key_down                       (kb_scancode scancode)                              { return state_curr.keyboard[scancode]; }
KB_API_INLINE   bool          kb_input_key_up                         (kb_scancode scancode)                              { return !state_curr.keyboard[scancode]; }
KB_API_INLINE   bool          kb_input_key_released                   (kb_scancode scancode)                              { return kb_deactivated(state_curr.keyboard, state_prev.keyboard, scancode); }
KB_API_INLINE   bool          kb_input_key_pressed                    (kb_scancode scancode)                              { return kb_activated(state_curr.keyboard, state_prev.keyboard, scancode); }
  
KB_API_INLINE   Float2        kb_input_mouse_wheel                    (void)                                              { return state_curr.wheel; }
KB_API_INLINE   bool          kb_input_mouse_button_down              (kb_mouse_button button)                            { return state_curr.mouse[button]; }
KB_API_INLINE   bool          kb_input_mouse_button_up                (kb_mouse_button button)                            { return !state_curr.mouse[button]; }
KB_API_INLINE   bool          kb_input_mouse_button_released          (kb_mouse_button button)                            { return kb_deactivated(state_curr.mouse, state_prev.mouse, button); }
KB_API_INLINE   bool          kb_input_mouse_button_pressed           (kb_mouse_button button)                            { return kb_activated(state_curr.mouse, state_prev.mouse, button); }
KB_API_INLINE   Float2        kb_input_mouse_pos                      (void)                                              { return state_curr.mouse_pos; }
KB_API_INLINE   Float2        kb_input_mouse_delta                    (void)                                              { return sub_float2(state_curr.mouse_pos, state_prev.mouse_pos); }

KB_API_INLINE   bool          kb_input_gamepad_button_down            (uint32_t gamepad, kb_gamepad_button button)        { return state_curr.gamepad[gamepad].button[button]; }
KB_API_INLINE   bool          kb_input_gamepad_button_up              (uint32_t gamepad, kb_gamepad_button button)        { return !kb_input_gamepad_button_down(gamepad, button); }
KB_API_INLINE   bool          kb_input_gamepad_button_released        (uint32_t gamepad, kb_gamepad_button button)        { return kb_deactivated(state_curr.gamepad[gamepad].button, state_prev.gamepad[gamepad].button, button); }
KB_API_INLINE   bool          kb_input_gamepad_button_pressed         (uint32_t gamepad, kb_gamepad_button button)        { return kb_activated(state_curr.gamepad[gamepad].button, state_prev.gamepad[gamepad].button, button); }
KB_API          bool          kb_input_gamepad_connected              (uint32_t gamepad);
KB_API          const char*   kb_input_gamepad_name                   (uint32_t gamepad);
  
KB_API_INLINE   float         kb_input_gamepad_axis_value             (uint32_t gamepad, kb_gamepad_axis axis)            { return state_curr.gamepad[gamepad].axis[axis]; }
KB_API_INLINE   float         kb_input_gamepad_axis_delta             (uint32_t gamepad, kb_gamepad_axis axis)            { return state_curr.gamepad[gamepad].axis[axis] - state_prev.gamepad[gamepad].axis[axis]; }
KB_API_INLINE   float         kb_input_gamepad_axis_distance          (uint32_t gamepad, kb_gamepad_axis axis)            { return abs_scalar(kb_input_gamepad_axis_delta(gamepad, axis)); }

KB_API_INLINE   Float2        kb_input_gamepad_joystick_axis_value    (uint32_t gamepad, kb_gamepad_joystick_axis axis)   { return (Float2) { kb_input_gamepad_axis_value(gamepad, axis.x), kb_input_gamepad_axis_value(gamepad, axis.y) }; }
KB_API_INLINE   Float2        kb_input_gamepad_joystick_axis_delta    (uint32_t gamepad, kb_gamepad_joystick_axis axis)   { return (Float2) { kb_input_gamepad_axis_delta(gamepad, axis.x), kb_input_gamepad_axis_delta(gamepad, axis.y) }; }
KB_API_INLINE   float         kb_input_gamepad_joystick_axis_distance (uint32_t gamepad, kb_gamepad_joystick_axis axis)   { return len_float2(kb_input_gamepad_joystick_axis_delta(gamepad, axis)); }

KB_API_INLINE   Float2        kb_input_gamepad_joystick_value         (uint32_t gamepad, kb_gamepad_joystick joystick)    { return kb_input_gamepad_joystick_axis_value(gamepad, joystick_axis[joystick]); }
KB_API_INLINE   Float2        kb_input_gamepad_joystick_delta         (uint32_t gamepad, kb_gamepad_joystick joystick)    { return kb_input_gamepad_joystick_axis_delta(gamepad, joystick_axis[joystick]); }
KB_API_INLINE   float         kb_input_gamepad_joystick_distance      (uint32_t gamepad, kb_gamepad_joystick joystick)    { return kb_input_gamepad_joystick_axis_distance(gamepad, joystick_axis[joystick]); }

//KB_API          uint32_t      kb_input_event_gamepad_connect        (void);
//KB_API          uint32_t      kb_input_event_gamepad_disconnect     (void);

#ifdef __cplusplus
}
#endif
