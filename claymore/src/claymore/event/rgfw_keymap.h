#ifndef __CLAYMORE_KEYMAP_H__
#define __CLAYMORE_KEYMAP_H__

#include "claymore/defines.h"

#define RGFW_NO_API
#define u8 u8
#define RGFWDEF
#include <RGFW.h>

typedef enum {
  CM_KEY_ESCAPE = RGFW_Escape,
  CM_KEY_F1 = RGFW_F1,
  CM_KEY_F2 = RGFW_F2,
  CM_KEY_F3 = RGFW_F3,
  CM_KEY_F4 = RGFW_F4,
  CM_KEY_F5 = RGFW_F5,
  CM_KEY_F6 = RGFW_F6,
  CM_KEY_F7 = RGFW_F7,
  CM_KEY_F8 = RGFW_F8,
  CM_KEY_F9 = RGFW_F9,
  CM_KEY_F10 = RGFW_F10,
  CM_KEY_F11 = RGFW_F11,
  CM_KEY_F12 = RGFW_F12,
  CM_KEY_F13 = RGFW_F13,
  CM_KEY_F14 = RGFW_F14,
  CM_KEY_F15 = RGFW_F15,

  CM_KEY_BACKTICK = RGFW_Backtick,

  CM_KEY_0 = RGFW_0,
  CM_KEY_1 = RGFW_1,
  CM_KEY_2 = RGFW_2,
  CM_KEY_3 = RGFW_3,
  CM_KEY_4 = RGFW_4,
  CM_KEY_5 = RGFW_5,
  CM_KEY_6 = RGFW_6,
  CM_KEY_7 = RGFW_7,
  CM_KEY_8 = RGFW_8,
  CM_KEY_9 = RGFW_9,

  CM_KEY_MINUS = RGFW_Minus,
  CM_KEY_EQUALS = RGFW_Equals,
  CM_KEY_BACKSPACE = RGFW_BackSpace,
  CM_KEY_TAB = RGFW_Tab,
  CM_KEY_CAPSLOCK = RGFW_CapsLock,
  CM_KEY_SHIFTL = RGFW_ShiftL,
  CM_KEY_CONTROLL = RGFW_ControlL,
  CM_KEY_ALTL = RGFW_AltL,
  CM_KEY_SUPERL = RGFW_SuperL,
  CM_KEY_SHIFTR = RGFW_ShiftR,
  CM_KEY_CONTROLR = RGFW_ControlR,
  CM_KEY_ALTR = RGFW_AltR,
  CM_KEY_SUPERR = RGFW_SuperR,
  CM_KEY_SPACE = RGFW_Space,

  CM_KEY_A = RGFW_A,
  CM_KEY_B = RGFW_B,
  CM_KEY_C = RGFW_C,
  CM_KEY_D = RGFW_D,
  CM_KEY_E = RGFW_E,
  CM_KEY_F = RGFW_F,
  CM_KEY_G = RGFW_G,
  CM_KEY_H = RGFW_H,
  CM_KEY_I = RGFW_I,
  CM_KEY_J = RGFW_J,
  CM_KEY_K = RGFW_K,
  CM_KEY_L = RGFW_L,
  CM_KEY_M = RGFW_M,
  CM_KEY_N = RGFW_N,
  CM_KEY_O = RGFW_O,
  CM_KEY_P = RGFW_P,
  CM_KEY_Q = RGFW_Q,
  CM_KEY_R = RGFW_R,
  CM_KEY_S = RGFW_S,
  CM_KEY_T = RGFW_T,
  CM_KEY_U = RGFW_U,
  CM_KEY_V = RGFW_V,
  CM_KEY_W = RGFW_W,
  CM_KEY_X = RGFW_X,
  CM_KEY_Y = RGFW_Y,
  CM_KEY_Z = RGFW_Z,

  CM_KEY_PERIOD = RGFW_Period,
  CM_KEY_COMMA = RGFW_Comma,
  CM_KEY_SLASH = RGFW_Slash,
  CM_KEY_BRACKET = RGFW_Bracket,
  CM_KEY_CLOSEBRACKET = RGFW_CloseBracket,
  CM_KEY_SEMICOLON = RGFW_Semicolon,
  CM_KEY_RETURN = RGFW_Return,
  CM_KEY_QUOTE = RGFW_Quote,
  CM_KEY_BACKSLASH = RGFW_BackSlash,

  CM_KEY_UP = RGFW_Up,
  CM_KEY_DOWN = RGFW_Down,
  CM_KEY_LEFT = RGFW_Left,
  CM_KEY_RIGHT = RGFW_Right,

  CM_KEY_DELETE = RGFW_Delete,
  CM_KEY_INSERT = RGFW_Insert,
  CM_KEY_END = RGFW_End,
  CM_KEY_HOME = RGFW_Home,
  CM_KEY_PAGEUP = RGFW_PageUp,
  CM_KEY_PAGEDOWN = RGFW_PageDown,

  CM_KEY_NUMLOCK = RGFW_Numlock,
  CM_KEY_KP_SLASH = RGFW_KP_Slash,
  CM_KEY_MULTIPLY = RGFW_Multiply,
  CM_KEY_KP_MINUS = RGFW_KP_Minus,
  CM_KEY_KP_1 = RGFW_KP_1,
  CM_KEY_KP_2 = RGFW_KP_2,
  CM_KEY_KP_3 = RGFW_KP_3,
  CM_KEY_KP_4 = RGFW_KP_4,
  CM_KEY_KP_5 = RGFW_KP_5,
  CM_KEY_KP_6 = RGFW_KP_6,
  CM_KEY_KP_7 = RGFW_KP_7,
  CM_KEY_KP_8 = RGFW_KP_8,
  CM_KEY_KP_9 = RGFW_KP_9,
  CM_KEY_KP_0 = RGFW_KP_0,
  CM_KEY_KP_PERIOD = RGFW_KP_Period,
  CM_KEY_KP_RETURN = RGFW_KP_Return,
} CmKeyCodes;

typedef enum {
  CM_KEY_PRESS = RGFW_keyPressed,    /* a key has been pressed */
  CM_KEY_RELEASE = RGFW_keyReleased, /*!< a key has been released*/
} CmKeyAction;

typedef enum {
  CM_MOUSE_BUTTON_LEFT = RGFW_mouseLeft,
  CM_MOUSE_BUTTON_RIGHT = RGFW_mouseLeft,
  CM_MOUSE_BUTTON_MIDDLE = RGFW_mouseMiddle,
} CmMouseButtons;

#endif /* !__CLAYMORE_KEYMAP_H__ */
