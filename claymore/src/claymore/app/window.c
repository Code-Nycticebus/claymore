#include "window.h"

#include "GLFW/glfw3.h"

static CmWindow window;

bool cm_window_create(usize width, usize height, const char *title) {
  if (!glfwInit()) {
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window.context = glfwCreateWindow((u32)width, (u32)height, title, NULL, NULL);
  if (window.context == NULL) {
    return false;
  }

  return true;
}

void *cm_window_context(void) { return window.context; }
double cm_window_time(void) { return glfwGetTime(); }

void cm_window_set_bg_color(vec3 color) { glClearColor(VEC3_ARG(color), 1); }

void cm_window_close(void) {
  glfwDestroyWindow(window.context);
  glfwTerminate();
}

bool cm_window_should_close(void) {
  return glfwWindowShouldClose(window.context);
}

void cm_window_swap_buffers(void) { glfwSwapBuffers(window.context); }
void cm_window_poll_events(void) { glfwPollEvents(); }
