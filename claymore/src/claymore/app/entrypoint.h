#ifndef __CLAYMORE_ENTRYPOINT__
#define __CLAYMORE_ENTRYPOINT__

#include "claymore/defines.h" // IWYU pragma: export

#include "renderer/context.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

typedef struct {
  struct {
    const char *title;
    usize width;
    usize height;
  } window;
} ClaymoreConfig;

ClaymoreConfig claymore_init(void);

int main(void) {
  ClaymoreConfig config = claymore_init();

  if (!glfwInit()) {
    clib_log_error("glfw could not get initialized!");
    return -1;
  }

  GLFWwindow *window =
      glfwCreateWindow((u32)config.window.width, (u32)config.window.height,
                       config.window.title, NULL, NULL);
  if (window == NULL) {
    clib_log_error("could not open window!");
    return -3;
  }

  cm_platform_context_init(window);

  glClearColor(1, 0, 0, 1);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}

#else
#error "Entrypoint included twice"
#endif
