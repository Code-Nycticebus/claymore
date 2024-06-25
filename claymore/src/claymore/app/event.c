#include "event.h"

#include "claymore/app/app.h"

void cm_event_emit(CmEvent event) { cm_app_internal_event(&event); }

void cm_event_internal_poll_events(RGFW_window *window) { // NOLINT
  while (RGFW_window_checkEvent(window)) {
    RGFW_Event *event = &window->event;

    // quit
    if (event->type == RGFW_quit) {
      cm_event_emit((CmEvent){
          .type = CM_EVENT_QUIT,
          .event = {{0}},
      });
      continue;
    }

    // key pressed
    if (event->type == RGFW_keyPressed || event->type == RGFW_keyReleased) {
      cm_event_emit((CmEvent){
          .type = CM_EVENT_KEY,
          .event.key =
              {
                  .key = str_from_cstr(window->event.keyName),
                  .code = window->event.keyCode,
                  .action = window->event.type,
                  .lock = window->event.lockState,
              },
      });
      continue;
    }

    // cursor
    if (event->type == RGFW_mousePosChanged) {
      vec2 pos = {event->point.x, event->point.y};
      static vec2 last_pos = {0};
      cm_event_emit((CmEvent){
          .type = CM_EVENT_CURSOR,
          .event.cursor =
              {
                  .direction = {last_pos[0] - pos[0], last_pos[1] - pos[1]},
                  .position = {pos[0], pos[1]},
              },
      });
      glm_vec2_copy(pos, last_pos);
      continue;
    }

    // scroll
    if (event->button == RGFW_mouseScrollUp ||
        event->button == RGFW_mouseScrollDown) {
      cm_event_emit((CmEvent){
          .type = CM_EVENT_SCROLL,
          .event.scroll = {.offset = {0, event->scroll}},
      });
      continue;
    }

    // mouse
    if (event->type == RGFW_mouseButtonPressed ||
        event->type == RGFW_mouseButtonReleased) {
      cm_event_emit((CmEvent){
          .type = CM_EVENT_MOUSE,
          .event.mouse =
              {
                  .action = event->type,
                  .button = event->button,
                  .position = {event->point.x, event->point.y},
              },
      });
      continue;
    }

    // controller buttons
    if (event->type == RGFW_jsButtonPressed ||
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
                  .controller = event->joystick,
              },
      });
      continue;
    }

    // controller joystick
    if (event->type == RGFW_jsAxisMove) {
      cm_event_emit((CmEvent){
          .type = CM_EVENT_JOYSTICK,
          .event.joystick =
              {
                  .controller = event->joystick,
                  .axis =
                      {
                          {event->axis[0].x, event->axis[0].y},
                          {event->axis[1].x, event->axis[1].y},
                      },
                  .button = event->button,
              },
      });
      continue;
    }

    // resize
    if (event->type == RGFW_windowResized) {
      glViewport(0, 0, window->r.w, window->r.h);
      cm_event_emit((CmEvent){
          .type = CM_EVENT_RESIZE,
          .event.resize = {.size = {window->r.w, window->r.h}},
      });
      continue;
    }

    // drop init
    if (event->type == RGFW_dnd_init) {
      continue; // ignore
    }

    // drop
    if (event->type == RGFW_dnd) {
      cm_event_emit((CmEvent){
          .type = CM_EVENT_DROP,
          .event.drop =
              {
                  .position = {event->point.x, event->point.y},
                  .count = event->droppedFilesCount,
                  .files = event->droppedFiles,
              },
      });
      continue;
    }

    // focus
    if (event->type == RGFW_focusIn || event->type == RGFW_focusOut) {
      cm_event_emit((CmEvent){
          .type = CM_EVENT_FOCUS,
          .event.focus = {.in_focus = (event->type == RGFW_focusIn)},
      });
      continue;
    }

    // move
    if (event->type == RGFW_windowMoved) {
      cm_event_emit((CmEvent){
          .type = CM_EVENT_MOVE,
          .event.move =
              {
                  .position = {window->r.x, window->r.y},
                  .size = {window->r.w, window->r.h},
              },
      });
      continue;
    }

    // event not handled yet
    cebus_log_error("event (%d) not implemented", event->type);
  }
}
