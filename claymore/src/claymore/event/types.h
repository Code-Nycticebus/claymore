#ifndef __CLAYMORE_EVENT_H__
#define __CLAYMORE_EVENT_H__

#include "claymore/defines.h" // IWYU pragma: export

#include "glfw_keymap.h" // IWYU pragma: export

typedef enum {
  CM_EVENT_MOUSE,
  CM_EVENT_CURSOR,
  CM_EVENT_KEY,
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

typedef union {
  CmEventKey key;
  CmEventCursor cursor;
  CmEventMouse mouse;
} CmEventData;

typedef struct {
  bool handled;
  CmEventType type;
  CmEventData data;
} CmEvent;

#endif /* !__CLAYMORE_EVENT_H__ */
