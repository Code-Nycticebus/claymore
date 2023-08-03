#include "event.h"
#include "claymore/core/app.h"
#include "claymore/core/window.h"
#include "claymore/events/mouse.h"
#include "claymore/logger/logger.h"

#include <assert.h>

#define CM_EVENT_MAX_CALLBACKS 16

typedef struct {
  App *app;
  uint32_t count;
  cm_event_callback callback[CM_EVENT_MAX_CALLBACKS];
} CmEventPool;

static CmEventPool cbs[] = {
    [CM_EVENT_MOUSE] = {0},         //
    [CM_EVENT_KEYBOARD] = {0},      //
    [CM_EVENT_WINDOW_RESIZE] = {0}, //
    [CM_EVENT_WINDOW_CLOSE] = {0},  //
    [CM_EVENT_SCROLL] = {0},
};

static struct {
  App *app;
} event_handler;

void cm_event_init(App *app) { event_handler.app = app; }

void cm_event_set_callback(CmEventType type, cm_event_callback callback) {
  assert(CM_EVENT_MOUSE <= type && type <= CM_EVENT_SCROLL);
  assert(cbs[type].count < CM_EVENT_MAX_CALLBACKS);
  cbs[type].callback[cbs[type].count] = callback;
  cbs[type].count += 1;
}

void cm_event_dispatch(CmEvent event) {
  for (int32_t i = cbs[event.type].count - 1; 0 <= i; --i) {
    cbs[event.type].callback[i](event_handler.app, &event.event);
    if (event.event.base.handled) {
      break;
    }
  }
}
