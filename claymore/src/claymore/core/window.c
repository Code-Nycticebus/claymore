#include "window.h"

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

float cm_window_time(void) { return glfwGetTime(); }

void cm_window_input_handler(CMwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

static void _cm_window_resize_callback(GLFWwindow *window, int32_t width,
                                       int32_t height) {
  (void)window;
  glViewport(0, 0, width, height);
}

CMwindow *cm_window_init(uint32_t width, uint32_t height, const char *name) {
  GLFWwindow *window;

  /* Initialize the library */
  if (!glfwInit()) {
    return NULL;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(width, height, name, NULL, NULL);
  if (!window) {
    glfwTerminate();
    return NULL;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(window, _cm_window_resize_callback);

  return window;
}
