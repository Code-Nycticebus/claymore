#include "window.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <stdio.h>

#include "cm.h"

#include "claymore/events/event.h"
#include "claymore/events/event_types.h"
#include "claymore/events/mouse.h"
#include "claymore/renderer/context.h"

// For now only one window is allowed!
static struct {
  CMwindow window;
} window_manager;

float cm_window_time(void) { return glfwGetTime(); }

void _cm_window_mouse_button_callback(GLFWwindow *window, int button,
                                      int action, int mods) {
  (void)window, (void)mods;
  const int cm_action =
      action == GLFW_PRESS ? CM_MOUSE_CLICK : CM_MOUSE_RELEASE;
  cm_mouseinfo_set_button(cm_action, button);
  cm_event_dispatch((CmEvent){.type = CM_EVENT_MOUSE,
                              .event.mouse = {
                                  .action = cm_action,
                                  .info = cm_mouseinfo(),
                              }});
}

static void _cm_window_mouse_pos_callback(GLFWwindow *glfw_window, double xpos,
                                          double ypos) {
  CMwindow *window = glfwGetWindowUserPointer(glfw_window);
  cm_mouseinfo_set_pos((vec2){xpos, window->height - ypos});
  cm_event_dispatch((CmEvent){.type = CM_EVENT_MOUSE,
                              .event.mouse = {
                                  .action = CM_MOUSE_MOVE,
                                  .info = cm_mouseinfo(),
                              }});
}

static void _cm_window_key_callback(GLFWwindow *window, int key, int scancode,
                                    int action, int mods) {
  (void)window, (void)scancode, (void)mods;
  const int cm_action =
      (action == GLFW_PRESS)
          ? CM_KEY_PRESS
          : (action == GLFW_REPEAT ? CM_KEY_REPEAT : CM_KEY_RELEASE);
  cm_key_set(key, cm_action);
  cm_event_dispatch((CmEvent){.type = CM_EVENT_KEYBOARD,
                              .event.key = {
                                  .action = cm_action,
                                  .code = key,
                              }});
}

static void _cm_window_resize_callback(GLFWwindow *glfw_window, int32_t width,
                                       int32_t height) {
  CMwindow *window = glfwGetWindowUserPointer(glfw_window);
  window->height = height;
  window->width = width;
  glViewport(0, 0, width, height);
  cm_event_dispatch((CmEvent){
      .type = CM_EVENT_WINDOW_RESIZE,
      .event.window = {.window = window},
  });
}

void _cm_window_close_callback(GLFWwindow *glfw_window) {
  CMwindow *window = glfwGetWindowUserPointer(glfw_window);
  cm_event_dispatch((CmEvent){
      .type = CM_EVENT_WINDOW_CLOSE,
      .event.window = {.window = window},
  });
}

void _cm_window_scroll_callback(GLFWwindow *glfw_window, double xoffset,
                                double yoffset) {
  CMwindow *window = glfwGetWindowUserPointer(glfw_window);
  cm_event_dispatch((CmEvent){
      .type = CM_EVENT_SCROLL,
      .event.scroll =
          {
              .window = window,
              .xoffset = xoffset,
              .yoffset = yoffset,
          },
  });
}

CMwindow *cm_window_init(uint32_t width, uint32_t height, const char *title) {
  GLFWwindow *window;

  static bool glfw_initialized = false;
  if (glfw_initialized == false) {
    /* Initialize the library */
    if (!glfwInit()) {
      return NULL;
    }
    glfw_initialized = true;
  } else {
    assert(0 && "Only one window allowed at the time!");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!window) {
    glfwTerminate();
    return NULL;
  }

  cm_context_init(window);

  glfwSetKeyCallback(window, _cm_window_key_callback);
  glfwSetCursorPosCallback(window, _cm_window_mouse_pos_callback);
  glfwSetMouseButtonCallback(window, _cm_window_mouse_button_callback);
  glfwSetFramebufferSizeCallback(window, _cm_window_resize_callback);
  glfwSetWindowCloseCallback(window, _cm_window_close_callback);
  glfwSetScrollCallback(window, _cm_window_scroll_callback);

  window_manager.window.height = height;
  window_manager.window.width = width;
  window_manager.window.title = title;
  window_manager.window.ctx = window;

  glfwSetWindowUserPointer(window, &window_manager.window);
  return &window_manager.window;
}

void cm_window_update(const CMwindow *window) {
  glfwSwapBuffers(window->ctx);
  glfwPollEvents();
}

void cm_window_shutdown(void) { glfwTerminate(); }

void cm_window_close(CMwindow *window) { glfwDestroyWindow(window->ctx); }
