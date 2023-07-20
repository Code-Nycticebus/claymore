#include "window.h"

#include "GL/glew.h"
#include "cglm/vec2.h"
#include "claymore/events/event.h"
#include "claymore/events/keymap.h"
#include "claymore/events/mouse.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

float cm_window_time(void) { return glfwGetTime(); }

void _cm_window_mouse_button_callback(GLFWwindow *window, int button,
                                      int action, int mods) {
  (void)window, (void)mods, (void)action;

  cm_event_dispatch((CmEvent){
      .type = CM_EVENT_MOUSE,
      .event.mouse.action =
          action == GLFW_PRESS ? CM_MOUSE_CLICK : CM_MOUSE_RELEASE,
      .event.mouse.info.button = button,
  });
}

static size_t window_heigth = 0;
static void _cm_window_mouse_pos_callback(GLFWwindow *window, double xpos,
                                          double ypos) {
  (void)window;
  cm_event_dispatch((CmEvent){
      .type = CM_EVENT_MOUSE,
      .event.mouse.action = CM_MOUSE_MOVE,
      .event.mouse.info.pos = {xpos, window_heigth - ypos},
  });
}

static void _cm_window_key_callback(GLFWwindow *window, int key, int scancode,
                                    int action, int mods) {
  (void)window, (void)scancode, (void)mods;

  cm_event_dispatch((CmEvent){
      .type = CM_EVENT_KEYBOARD,
      .event.key.action =
          (action == GLFW_PRESS)
              ? CM_KEY_PRESS
              : (action == GLFW_REPEAT ? CM_KEY_REPEAT : CM_KEY_RELEASE),
      .event.key.code = cm_keymap_translate(key),
  });
}
static void _cm_window_resize_callback(GLFWwindow *window, int32_t width,
                                       int32_t height) {
  (void)window;
  glViewport(0, 0, width, height);
}

CMwindow *cm_window_init(uint32_t width, uint32_t height, const char *name) {
  GLFWwindow *window;

  window_heigth = height;

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

  glfwSetKeyCallback(window, _cm_window_key_callback);
  glfwSetCursorPosCallback(window, _cm_window_mouse_pos_callback);
  glfwSetMouseButtonCallback(window, _cm_window_mouse_button_callback);
  return window;
}
