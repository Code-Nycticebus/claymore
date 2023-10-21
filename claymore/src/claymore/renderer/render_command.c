#include "render_command.h"

void cm_renderer_clear(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void cm_renderer_set_clear_color(vec4s color) {
  glClearColor(color.r, color.g, color.b, color.a);
}
