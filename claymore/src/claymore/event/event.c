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
