#ifndef __CLAYMORE_EVENT_H__
#define __CLAYMORE_EVENT_H__

#include "claymore/defines.h" // IWYU pragma: export

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

  CM_KEY_A = RGFW_a,
  CM_KEY_B = RGFW_b,
  CM_KEY_C = RGFW_c,
  CM_KEY_D = RGFW_d,
  CM_KEY_E = RGFW_e,
  CM_KEY_F = RGFW_f,
  CM_KEY_G = RGFW_g,
  CM_KEY_H = RGFW_h,
  CM_KEY_I = RGFW_i,
  CM_KEY_J = RGFW_j,
  CM_KEY_K = RGFW_k,
  CM_KEY_L = RGFW_l,
  CM_KEY_M = RGFW_m,
  CM_KEY_N = RGFW_n,
  CM_KEY_O = RGFW_o,
  CM_KEY_P = RGFW_p,
  CM_KEY_Q = RGFW_q,
  CM_KEY_R = RGFW_r,
  CM_KEY_S = RGFW_s,
  CM_KEY_T = RGFW_t,
  CM_KEY_U = RGFW_u,
  CM_KEY_V = RGFW_v,
  CM_KEY_W = RGFW_w,
  CM_KEY_X = RGFW_x,
  CM_KEY_Y = RGFW_y,
  CM_KEY_Z = RGFW_z,

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
  CM_KEY_PRESS = RGFW_keyPressed,
  CM_KEY_RELEASE = RGFW_keyReleased,
} CmKeyAction;

typedef enum {
  CM_MOUSE_BUTTON_LEFT = RGFW_mouseLeft,
  CM_MOUSE_BUTTON_RIGHT = RGFW_mouseLeft,
  CM_MOUSE_BUTTON_MIDDLE = RGFW_mouseMiddle,
} CmMouseButtons;

typedef enum {
  CM_EVENT_KEY,
  CM_EVENT_MOUSE,
  CM_EVENT_CURSOR,
  CM_EVENT_SCROLL,
  CM_EVENT_RESIZE,
  CM_EVENT_DROP,
} CmEventType;

typedef struct {
  CmKeyCodes code;
  CmKeyAction action;
} CmEventKey;

typedef struct {
  CmMouseButtons button;
  CmKeyAction action;
  vec2 pos;
} CmEventMouse;

typedef struct {
  vec2 pos;
} CmEventCursor;

typedef struct {
  vec2 offset;
} CmEventScroll;

typedef struct {
  vec2 size;
} CmEventResize;

typedef struct {
  size_t count;
  const char **files;
} CmEventDrop;

typedef union {
  CmEventKey key;
  CmEventCursor cursor;
  CmEventMouse mouse;
  CmEventScroll scroll;
  CmEventResize resize;
  CmEventDrop drop;
} CmEventData;

typedef struct {
  bool handled;
  CmEventType type;
  CmEventData event;
} CmEvent;

#endif /* !__CLAYMORE_EVENT_H__ */
