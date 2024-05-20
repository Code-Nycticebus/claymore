#include "window.h"

#include <glad.h>

#define u8 u8
#define RGFWDEF
#include <RGFW.h>

#include "event.h"

static struct {
  RGFW_window *context;
} window;

void *cm_window_context(void) { return window.context; }

double cm_window_time(void) {
  const double ns = 1e9;
  return RGFW_getTimeNS() / ns;
}

void cm_window_set_bg(const vec3 color) { glClearColor(VEC3_ARG(color), 1); }

void cm_window_get_size(vec2 out) {
  out[0] = window.context->r.w;
  out[1] = window.context->r.h;
}

void cm_window_close(void) { RGFW_window_setShouldClose(window.context); }

bool cm_window_internal_create(usize width, usize height, const char *title) {
  window.context = RGFW_createWindow(title, RGFW_RECT(0, 0, width, height), 0);
  if (window.context == NULL) {
    return false;
  }

  return true;
}

bool cm_window_internal_should_close(void) {
  return window.context->event.type == RGFW_quit;
}

void cm_window_internal_close(void) { RGFW_window_close(window.context); }

void cm_window_internal_swap_buffers(void) {
  RGFW_window_swapBuffers(window.context);
}

void cm_window_internal_poll_events(void) {
  while (RGFW_window_checkEvent(window.context)) {
    RGFW_Event *event = &window.context->event;
    if (event->type == RGFW_quit) {
      cm_window_close();
      return;
    }
    if (event->type == RGFW_keyPressed || event->type == RGFW_keyReleased) {
      cm_event_emit((CmEvent){
          .type = CM_EVENT_KEY,
          .event.key =
              {
                  .code = window.context->event.keyCode,
                  .action = window.context->event.type,
              },
      });
    } else if (event->type == RGFW_mousePosChanged) {
      cm_event_emit((CmEvent){
          .type = CM_EVENT_CURSOR,
          .event.cursor = {.pos = {event->point.x, event->point.y}},
      });
    } else if (event->type == RGFW_windowAttribsChange) {
      cm_event_emit((CmEvent){
          .type = CM_EVENT_RESIZE,
          .event.resize = {.size = {window.context->r.w, window.context->r.h}},
      });
    } else if (event->type == RGFW_mouseButtonPressed ||
               event->type == RGFW_mouseButtonReleased) {
      if (event->button == RGFW_mouseScrollUp ||
          event->button == RGFW_mouseScrollDown) {
        cm_event_emit((CmEvent){
            .type = CM_EVENT_SCROLL,
            .event.scroll = {.offset = {0, event->scroll}},
        });
      } else {
        cm_event_emit((CmEvent){
            .type = CM_EVENT_MOUSE,
            .event.mouse =
                {
                    .action = event->type == RGFW_mouseButtonPressed
                                  ? CM_KEY_PRESS
                                  : CM_KEY_RELEASE,
                    .button = event->button,
                    .pos = {event->point.x, event->point.y},
                },
        });
      }
    } else {
      cebus_log_error("EVENT %d", event->type);
      NOT_IMPLEMENTED();
    }
  }
}
