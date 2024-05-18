#ifndef __CLAYMORE_EVENTS_H__
#define __CLAYMORE_EVENTS_H__

#include "claymore/defines.h" // IWYU pragma: export
#include "types.h"

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

bool cm_event_key_pressed(CmKeyCodes key);
CmKeyAction cm_event_mouse_button_pressed(CmMouseButtons button);
void cm_event_cursor_position(vec2 out);

#endif /* !__CLAYMORE_EVENTS_H__ */
