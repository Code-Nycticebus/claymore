#include "event.h"
#include "claymore/core/app.h"
#include "claymore/core/window.h"
#include "claymore/logger/logger.h"

#include <assert.h>
#include <bits/stdint-uintn.h>

#define CM_EVENT_MAX_CALLBACKS 16

typedef struct {
  uint32_t count;
  cm_event_callback callback[CM_EVENT_MAX_CALLBACKS];
} CmEventPool;

static CmEventPool cbs[] = {
    [CM_EVENT_MOUSE] = {0},
    [CM_EVENT_KEYBOARD] = {0},
    [CM_EVENT_WINDOW_RESIZE] = {0},
};

void cm_event_init(void) {}

void cm_event_set_callback(CmEventType type, cm_event_callback callback) {
  assert(cbs[type].count < CM_EVENT_MAX_CALLBACKS);
  cbs[type].callback[cbs[type].count] = callback;
  cbs[type].count += 1;
}

void cm_event_dispatch(CmEvent event) {
  if (event.type == CM_EVENT_KEYBOARD) {
    cm_key_set(event.event.key.code, event.event.key.action);
  }
  assert(cbs[event.type].callback);
  for (int32_t i = cbs[event.type].count - 1; 0 <= i; --i) {
    if (!event.event.base.handled) {
      cbs[event.type].callback[i](&event.event);
    }
  }
}
