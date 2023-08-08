#include "event.h"
#include "claymore/core/app.h"
#include "claymore/core/window.h"
#include "claymore/events/mouse.h"
#include "claymore/logger/logger.h"

#include <assert.h>

#define CM_EVENT_MAX_CALLBACKS 16

typedef struct {
  uint32_t count;
  struct {
    void *data;
    cm_event_callback fn;
  } callback[CM_EVENT_MAX_CALLBACKS];
} CmEventPool;

static CmEventPool cbs[] = {
    [CM_EVENT_MOUSE] = {0},         //
    [CM_EVENT_KEYBOARD] = {0},      //
    [CM_EVENT_WINDOW_RESIZE] = {0}, //
    [CM_EVENT_WINDOW_CLOSE] = {0},  //
    [CM_EVENT_SCROLL] = {0},
};

void cm_event_set_callback(void *data, CmEventType type,
                           cm_event_callback callback) {
  assert(CM_EVENT_MOUSE <= type && type <= CM_EVENT_SCROLL);
  assert(cbs[type].count < CM_EVENT_MAX_CALLBACKS);
  cbs[type].callback[cbs[type].count].data = data;
  cbs[type].callback[cbs[type].count].fn = callback;
  cbs[type].count += 1;
}

void cm_event_dispatch(CmEvent event) {
  for (int32_t i = cbs[event.type].count - 1; 0 <= i; --i) {
    cbs[event.type].callback[i].fn(cbs[event.type].callback[i].data,
                                   &event.event);
    if (event.event.base.handled) {
      break;
    }
  }
}
