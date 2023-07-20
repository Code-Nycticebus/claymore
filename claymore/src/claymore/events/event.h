#ifndef __CM_EVENTS_H__
#define __CM_EVENTS_H__

#include <stdbool.h>

#include "key.h"
#include "keycodes.h"
#include "mouse.h"

typedef enum {
  CM_EVENT_MOUSE = 0,
  CM_EVENT_KEYBOARD,
  CM_EVENT_WINDOW_RESIZE,
} CmEventType;

typedef union CmEventUnion {
  CmKeyEvent key;
  CmMouseEvent mouse;
} CmEventUnion;

typedef struct {
  CmEventType type;
  CmEventUnion event;
} CmEvent;

typedef void (*cm_event_callback)(const CmEventUnion *e);

void cm_event_init(void);
void cm_event_dispatch(CmEvent event);
void cm_event_set_callback(CmEventType type, cm_event_callback callback);

#endif /* !__CM_EVENTS_H__ */
