#include "mouse.h"
#include <stdio.h>

static CmMouseInfo mouse_info;

CmMouseInfo cm_mouseinfo(void) { return mouse_info; }

void cm_mouseinfo_pos(vec2 pos) {
  pos[0] = mouse_info.x;
  pos[1] = mouse_info.y;
}

void cm_mouseinfo_set_pos(float x, float y) {
  mouse_info.x = x;
  mouse_info.y = y;
}

void cm_mouseinfo_set_button(CmKeyAction action, CmMouseKeycode button) {
  mouse_info.action = action;
  mouse_info.button = button;
}
