#include "event.h"

#include "claymore/app/app.h"
#include "claymore/app/window.h"

#include <GLFW/glfw3.h>

void cm_event_emit(CmEvent event) { app_internal_event(&event); }

CmKeyAction cm_event_key_pressed(CmKeyCodes key) {
  return (CmKeyAction)glfwGetKey(cm_window_context(), (int)key);
}

CmKeyAction cm_event_mouse_button_pressed(CmMouseButtons button) {
  return (CmKeyAction)glfwGetMouseButton(cm_window_context(), (int)button);
}

void cm_event_cursor_position(vec2 out) {
  double pos[2];
  glfwGetCursorPos(cm_window_context(), &pos[0], &pos[1]);
  out[0] = pos[0];
  out[1] = pos[1];
}
