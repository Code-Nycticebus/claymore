#ifndef __CLAYMORE_EVENT_H__
#define __CLAYMORE_EVENT_H__

#include "claymore/defines.h" // IWYU pragma: export

#include "rgfw_keymap.h"

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
