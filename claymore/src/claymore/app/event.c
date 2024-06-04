#include "event.h"

#include "claymore/app/app.h"

void cm_event_emit(CmEvent event) { cm_app_internal_event(&event); }

void cm_event_internal_poll_events(RGFW_window *window) {
  while (RGFW_window_checkEvent(window)) {
    RGFW_Event *event = &window->event;
    if (event->type == RGFW_quit) {
      cm_event_emit((CmEvent){
          .type = CM_EVENT_QUIT,
          .event = {{0}},
      });
    } else if (event->type == RGFW_keyPressed ||
               event->type == RGFW_keyReleased) {
      cm_event_emit((CmEvent){
          .type = CM_EVENT_KEY,
          .event.key =
              {
                  .code = window->event.keyCode,
                  .action = window->event.type,
              },
      });
    } else if (event->type == RGFW_mousePosChanged) {
      vec2 pos = {event->point.x, event->point.y};
      static vec2 last_pos = {0};
      cm_event_emit((CmEvent){
          .type = CM_EVENT_CURSOR,
          .event.cursor =
              {
                  .dir = {last_pos[0] - pos[0], last_pos[1] - pos[1]},
                  .pos = {pos[0], pos[1]},
              },
      });
      glm_vec2_copy(pos, last_pos);
    } else if (event->type == RGFW_windowAttribsChange) {
      cm_event_emit((CmEvent){
          .type = CM_EVENT_RESIZE,
          .event.resize = {.size = {window->r.w, window->r.h}},
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
                    .action = event->type,
                    .button = event->button,
                    .pos = {event->point.x, event->point.y},
                },
        });
      }
    } else if (event->type == RGFW_jsButtonPressed ||
               event->type == RGFW_jsButtonReleased) {
      cm_event_emit((CmEvent){
          .type = CM_EVENT_CONTROLLER,
          .event.controller =
              {
                  .action = event->type,
                  .button = event->button,
                  .axis =
                      {
                          {event->axis[0].x, event->axis[0].y},
                          {event->axis[1].x, event->axis[1].y},
                      },
              },
      });
    } else if (event->type == RGFW_jsAxisMove) {
      cm_event_emit((CmEvent){
          .type = CM_EVENT_JOYSTICK,
          .event.joystick =
              {
                  .joystick = event->joystick,
                  .axis =
                      {
                          {event->axis[0].x, event->axis[0].y},
                          {event->axis[1].x, event->axis[1].y},
                      },
              },
      });
    } else {
      cebus_log_error("EVENT %d", event->type);
      NOT_IMPLEMENTED();
    }
  }
}
