#include "event.h"

#include "claymore/app/app.h"
#include "claymore/app/window.h"

#define u8 u8
#define RGFWDEF
#include <RGFW.h>

void cm_event_emit(CmEvent event) { cm_app_internal_event(&event); }

bool cm_event_key_pressed(CmKeyCodes key) {
  return RGFW_isPressedI(cm_window_context(), key);
}

CmKeyAction cm_event_mouse_button_pressed(CmMouseButtons button) {
  (void)button;
  return 0;
}

void cm_event_cursor_position(vec2 out) {
  RGFW_window *win = cm_window_context();
  out[0] = win->event.point.x;
  out[1] = win->event.point.y;
}
