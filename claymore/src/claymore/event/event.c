#include "event.h"

#include "claymore/app/app.h"
#include "claymore/app/window.h"

#include <GLFW/glfw3.h>

void cm_event_emit(CmEvent event) { app_internal_on_event(&event); }

CmKeyAction cm_event_key_pressed(CmKeyCodes key) {
  return glfwGetKey(cm_window_context(), key);
}

CmKeyAction cm_event_mouse_button_pressed(CmMouseButtons button) {
  return glfwGetMouseButton(cm_window_context(), button);
}
