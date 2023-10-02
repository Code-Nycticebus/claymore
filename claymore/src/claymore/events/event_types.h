#ifndef __CM_EVENT_TYPE_H__
#define __CM_EVENT_TYPE_H__

#include "claymore/core/window.h"
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

typedef struct {
  vec2s pos;
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

typedef struct {
  struct CmEventBase base;
  CMwindow *window;
} CmWindowEvent;

typedef struct {
  struct CmEventBase base;
  CMwindow *window;
  double yoffset;
  double xoffset;
} CmScrollEvent;

typedef struct {
  size_t count;
  const char **files;
} CmDropEvent;

typedef enum {
  CM_EVENT_MOUSE,
  CM_EVENT_KEYBOARD,
  CM_EVENT_WINDOW_RESIZE,
  CM_EVENT_WINDOW_CLOSE,
  CM_EVENT_SCROLL,
  CM_EVENT_DROP,
  _CM_EVENT_COUNT,
} CmEventType;

typedef union {
  struct CmEventBase base;
  CmKeyEvent key;
  CmMouseEvent mouse;
  CmWindowEvent window;
  CmScrollEvent scroll;
  CmDropEvent drop;
} CmEventUnion;

typedef struct {
  CmEventType type;
  CmEventUnion event;
} CmEvent;

#endif /* !__CM_EVENT_TYPE_H__ */
