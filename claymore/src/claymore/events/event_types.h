#ifndef __CM_EVENT_TYPE_H__
#define __CM_EVENT_TYPE_H__

#include "cm.h"
#include "keycodes.h"

#include <stdbool.h>

struct CmEventBase {
  bool handled;
};

typedef enum {
  CM_KEY_RELEASE = 0,
  CM_KEY_PRESS,
  CM_KEY_REPEAT,
} CmKeyAction;

typedef struct {
  struct CmEventBase base;
  CmKeyAction action;
  CmKeyCodes code;
} CmKeyEvent;

typedef enum {
  CM_MOUSE_BUTTON_1,
  CM_MOUSE_BUTTON_2,
  CM_MOUSE_BUTTON_3,
  CM_MOUSE_BUTTON_4,
  CM_MOUSE_BUTTON_5,
  CM_MOUSE_BUTTON_6,
  CM_MOUSE_BUTTON_7,
  CM_MOUSE_BUTTON_8,
  CM_MOUSE_BUTTON_COUNT,
  CM_MOUSE_BUTTON_LEFT = CM_MOUSE_BUTTON_1,
  CM_MOUSE_BUTTON_RIGHT = CM_MOUSE_BUTTON_2,
  CM_MOUSE_BUTTON_MIDDLE = CM_MOUSE_BUTTON_3,
} CmMouseKeycode;

typedef struct {
  vec2 pos;
  CmMouseKeycode button;
} CmMouseInfo;

typedef enum {
  CM_MOUSE_RELEASE,
  CM_MOUSE_CLICK,
  CM_MOUSE_MOVE,
  CM_MOUSE_SCROLL,
} CmMouseAction;

typedef struct {
  struct CmEventBase base;
  CmMouseAction action;
  CmMouseInfo info;
} CmMouseEvent;

typedef union CmEventUnion {
  struct CmEventBase base;
  CmKeyEvent key;
  CmMouseEvent mouse;
} CmEventUnion;

typedef enum {
  CM_EVENT_MOUSE = 0,
  CM_EVENT_KEYBOARD,
  CM_EVENT_WINDOW_RESIZE,
} CmEventType;

typedef struct {
  CmEventType type;
  CmEventUnion event;
} CmEvent;

#endif /* !__CM_EVENT_TYPE_H__ */
