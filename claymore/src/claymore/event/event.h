#ifndef __CLAYMORE_EVENTS_H__
#define __CLAYMORE_EVENTS_H__

#include "claymore/defines.h" // IWYU pragma: export
#include "types.h"

#define _cm_event(event, name, MEMBER, T, TYPE, ...)                           \
  do {                                                                         \
    if ((event)->type == TYPE) {                                               \
      T *name = (T *)&(event)->data.MEMBER;                                    \
      __VA_ARGS__                                                              \
    }                                                                          \
  } while (0)

#define cm_event_mouse(event, name, ...)                                       \
  _cm_event(event, name, mouse, CmEventMouse, CM_EVENT_MOUSE, __VA_ARGS__)

#define cm_event_cursor(event, name, ...)                                      \
  _cm_event(event, name, cursor, CmEventCursor, CM_EVENT_CURSOR, __VA_ARGS__)

#define cm_event_key(event, name, ...)                                         \
  _cm_event(event, name, key, CmEventKey, CM_EVENT_KEY, __VA_ARGS__)

void cm_event_emit(CmEvent event);

CmKeyAction cm_event_key_pressed(CmKeyCodes key);
CmKeyAction cm_event_mouse_button_pressed(CmMouseButtons button);

#endif /* !__CLAYMORE_EVENTS_H__ */
