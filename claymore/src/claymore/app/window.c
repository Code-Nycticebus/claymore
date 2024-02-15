#include "window.h"

#include "claymore/event/event.h"

#include "GLFW/glfw3.h"

static CmWindow window;

static void keyboard_callback(GLFWwindow *context, int key, int scancode,
                              int action, int mods) {
  (void)context, (void)key, (void)scancode, (void)action, (void)mods;
  cm_event_emit((CmEvent){
      .type = CM_EVENT_KEY,
      .data.key = {.code = key, .action = action},
  });
}

static void mouse_callback(GLFWwindow *context, int button, int action,
                           int mods) {
  (void)mods;
  double pos[2];
  glfwGetCursorPos(context, &pos[0], &pos[1]);
  cm_event_emit((CmEvent){
      .type = CM_EVENT_MOUSE,
      .data.mouse =
          {
              .button = button,
              .action = action,
              .pos = {pos[0], pos[1]},
          },
  });
}

static void cursor_callback(GLFWwindow *context, double xpos, double ypos) {
  (void)context;
  cm_event_emit((CmEvent){
      .type = CM_EVENT_CURSOR,
      .data.cursor = {.pos = {xpos, ypos}},
  });
}

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

  glfwSetKeyCallback(window.context, keyboard_callback);
  glfwSetMouseButtonCallback(window.context, mouse_callback);
  glfwSetCursorPosCallback(window.context, cursor_callback);

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
