#include "event.h"
#include "claymore/core/app.h"
#include "claymore/core/window.h"
#include "claymore/events/mouse.h"
#include "claymore/logger/logger.h"

#include <assert.h>

#define CM_EVENT_MAX_CALLBACKS 16

typedef struct {
  uint32_t count;
  uint32_t top;
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
    [CM_EVENT_SCROLL] = {0},        //
    [CM_EVENT_DROP] = {0},
};
const size_t cbs_count = sizeof(cbs) / sizeof(cbs[0]);

void cm_event_top_set(void) {
  for (size_t i = 0; i < cbs_count; i++) {
    cbs[i].top = cbs[i].count;
  }
}

void cm_event_top_reset(void) {
  for (size_t i = 0; i < cbs_count; i++) {
    cbs[i].count = cbs[i].top;
  }
}

void cm_event_subscribe(CmEventType type, cm_event_callback callback,
                        void *data) {
  assert(CM_EVENT_MOUSE <= type && type <= CM_EVENT_DROP);
  assert(cbs[type].count < CM_EVENT_MAX_CALLBACKS);
  cbs[type].callback[cbs[type].count].data = data;
  cbs[type].callback[cbs[type].count].fn = callback;
  cbs[type].count += 1;
}

void cm_event_dispatch(CmEvent event) {
  for (int32_t i = cbs[event.type].count - 1; 0 <= i; --i) {
    cbs[event.type].callback[i].fn(&event.event,
                                   cbs[event.type].callback[i].data);
    if (event.event.base.handled) {
      break;
    }
  }
}
