#include "event.h"
#include "claymore/core/app.h"
#include "claymore/core/window.h"
#include "claymore/logger/logger.h"

#include <assert.h>

static cm_event_callback cbs[] = {
    [CM_EVENT_MOUSE_CLICK] = NULL,
    [CM_EVENT_MOUSE_MOVE] = NULL,
    [CM_EVENT_KEYBOARD] = NULL,
    [CM_EVENT_WINDOW_RESIZE] = NULL,
};

void cm_event_init(void) {}

void cm_event_set_callback(CmEventType type, cm_event_callback callback) {
  cbs[type] = callback;
}

void cm_event_dispatch(CmEvent event) {
  if (event.type == CM_EVENT_KEYBOARD) {
    cm_key_set(event.event.key.code, event.event.key.action);
  }
  cbs[event.type](&event.event);
}
