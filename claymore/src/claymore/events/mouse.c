#include "mouse.h"
#include <stdio.h>

static CmMouseAction mouse_buttons[CM_MOUSE_BUTTON_COUNT];
static CmMouseInfo mouse_info;

CmMouseInfo cm_mouseinfo(void) { return mouse_info; }

CmMouseAction cm_mouseinfo_button(CmMouseKeycode button) {
  assert(button < CM_MOUSE_BUTTON_COUNT);
  return mouse_buttons[button];
}

vec2s cm_mouseinfo_pos(void) { return mouse_info.pos; }

void cm_mouseinfo_set_pos(vec2s pos) { mouse_info.pos = pos; }

void cm_mouseinfo_set_button(CmMouseAction action, CmMouseKeycode button) {
  assert(button < CM_MOUSE_BUTTON_COUNT);
  mouse_buttons[button] = action;
  mouse_info.button = button;
}
