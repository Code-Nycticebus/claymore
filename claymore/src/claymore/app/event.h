#ifndef __CLAYMORE_EVENTS_H__
#define __CLAYMORE_EVENTS_H__

#include "claymore/defines.h" // IWYU pragma: export

typedef enum {
  CM_EVENT_KEY,
  CM_EVENT_MOUSE,
  CM_EVENT_CURSOR,
  CM_EVENT_SCROLL,
  CM_EVENT_RESIZE,
  CM_EVENT_DROP,
  CM_EVENT_QUIT,
} CmEventType;

typedef struct {
  u64 code;
  u64 action;
} CmEventKey;

typedef struct {
  u64 button;
  u64 action;
  vec2 pos;
} CmEventMouse;

typedef struct {
  vec2 pos;
  vec2 dir;
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

// typedef struct {} CmEventQuit;

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

#define _cm_event(e, MEMBER, T, TYPE, ...)                                     \
  do {                                                                         \
    if ((e)->type == TYPE) {                                                   \
      T *MEMBER = (T *)&(e)->event.MEMBER;                                     \
      __VA_ARGS__                                                              \
    }                                                                          \
  } while (0)

#define cm_event_key(e, ...)                                                   \
  _cm_event(e, key, CmEventKey, CM_EVENT_KEY, __VA_ARGS__)

#define cm_event_mouse(e, ...)                                                 \
  _cm_event(e, mouse, CmEventMouse, CM_EVENT_MOUSE, __VA_ARGS__)

#define cm_event_cursor(e, ...)                                                \
  _cm_event(e, cursor, CmEventCursor, CM_EVENT_CURSOR, __VA_ARGS__)

#define cm_event_scroll(e, ...)                                                \
  _cm_event(e, scroll, CmEventScroll, CM_EVENT_SCROLL, __VA_ARGS__)

#define cm_event_resize(e, ...)                                                \
  _cm_event(e, resize, CmEventResize, CM_EVENT_RESIZE, __VA_ARGS__)

#define cm_event_drop(e, ...)                                                  \
  _cm_event(e, drop, CmEventDrop, CM_EVENT_DROP, __VA_ARGS__)

void cm_event_emit(CmEvent event);

void cm_event_internal_poll_events(RGFW_window *window);

#endif /* !__CLAYMORE_EVENTS_H__ */
