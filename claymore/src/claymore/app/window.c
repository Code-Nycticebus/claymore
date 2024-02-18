#include "window.h"

#include "claymore/event/event.h"

#include <GLFW/glfw3.h>

static struct {
  void *context;
} window;

void *cm_window_context(void) { return window.context; }
double cm_window_time(void) { return glfwGetTime(); }
void cm_window_bg_color(vec3 color) { glClearColor(VEC3_ARG(color), 1); }
void cm_window_close(bool c) { glfwSetWindowShouldClose(window.context, c); }

static void keyboard_callback(GLFWwindow *context, int key, int scancode,
                              int action, int mods) {
  (void)context, (void)scancode, (void)mods;
  cm_event_emit((CmEvent){
      .type = CM_EVENT_KEY,
      .event.key = {.code = key, .action = action},
  });
}

static void mouse_callback(GLFWwindow *context, int btn, int action, int mods) {
  (void)mods;
  double pos[2];
  glfwGetCursorPos(context, &pos[0], &pos[1]);
  cm_event_emit((CmEvent){
      .type = CM_EVENT_MOUSE,
      .event.mouse =
          {
              .button = btn,
              .action = action,
              .pos = {pos[0], pos[1]},
          },
  });
}

static void cursor_callback(GLFWwindow *context, double x, double y) {
  (void)context;
  cm_event_emit((CmEvent){
      .type = CM_EVENT_CURSOR,
      .event.cursor = {.pos = {x, y}},
  });
}

static void scroll_callback(GLFWwindow *context, double x, double y) {
  (void)context;
  cm_event_emit((CmEvent){
      .type = CM_EVENT_SCROLL,
      .event.scroll = {.offset = {x, y}},
  });
}

static void resize_callback(GLFWwindow *context, i32 width, i32 height) {
  (void)context;
  glViewport(0, 0, width, height);
  cm_event_emit((CmEvent){
      .type = CM_EVENT_RESIZE,
      .event.resize = {.size = {width, height}},
  });
}

static void drop_callback(GLFWwindow *context, int count, const char **paths) {
  (void)context;
  cm_event_emit((CmEvent){
      .type = CM_EVENT_DROP,
      .event.drop =
          {
              .count = count,
              .files = paths,
          },
  });
}

bool cm_window_internal_create(usize width, usize height, const char *title) {
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

  glfwSetKeyCallback(window.context, keyboard_callback);
  glfwSetMouseButtonCallback(window.context, mouse_callback);
  glfwSetCursorPosCallback(window.context, cursor_callback);
  glfwSetScrollCallback(window.context, scroll_callback);
  glfwSetFramebufferSizeCallback(window.context, resize_callback);
  glfwSetDropCallback(window.context, drop_callback);

  return true;
}

void cm_window_internal_close(void) {
  glfwDestroyWindow(window.context);
  glfwTerminate();
}

bool cm_window_internal_should_close(void) {
  return glfwWindowShouldClose(window.context);
}

void cm_window_internal_swap_buffers(void) { glfwSwapBuffers(window.context); }
void cm_window_internal_poll_events(void) { glfwPollEvents(); }
