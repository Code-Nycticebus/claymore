#include "event.h"
#include "claymore/core/app.h"
#include "claymore/core/window.h"
#include "claymore/events/mouse.h"
#include "claymore/logger/logger.h"

#include <assert.h>
#include <bits/stdint-uintn.h>

#define CM_EVENT_MAX_CALLBACKS 16

typedef struct {
  App *app;
  uint32_t count;
  cm_event_callback callback[CM_EVENT_MAX_CALLBACKS];
} CmEventPool;

static CmEventPool cbs[] = {
    [CM_EVENT_MOUSE] = {0},
    [CM_EVENT_KEYBOARD] = {0},
    [CM_EVENT_WINDOW_RESIZE] = {0},
    [CM_EVENT_WINDOW_CLOSE] = {0},
};

static struct {
  App *app;
} event_handler;

void cm_event_init(App *app) { event_handler.app = app; }

void cm_event_set_callback(CmEventType type, cm_event_callback callback) {
  assert(cbs[type].count < CM_EVENT_MAX_CALLBACKS);
  cbs[type].callback[cbs[type].count] = callback;
  cbs[type].count += 1;
}

void cm_event_dispatch(CmEvent event) {
  switch (event.type) {
  case CM_EVENT_MOUSE: {
    if (event.event.mouse.action == CM_MOUSE_MOVE) {
      cm_mouseinfo_set_pos(event.event.mouse.info.pos);
    } else if (event.event.mouse.action == CM_MOUSE_CLICK ||
               event.event.mouse.action == CM_MOUSE_RELEASE) {
      cm_mouseinfo_set_button(event.event.mouse.action,
                              event.event.mouse.info.button);
    }
    event.event.mouse.info = cm_mouseinfo();
    break;
  }
  case CM_EVENT_KEYBOARD: {
    cm_key_set(event.event.key.code, event.event.key.action);
    break;
  }
  default:
    break;
  }

  for (int32_t i = cbs[event.type].count - 1; 0 <= i; --i) {
    if (!event.event.base.handled) {
      cbs[event.type].callback[i](event_handler.app, &event.event);
    }
  }
}
