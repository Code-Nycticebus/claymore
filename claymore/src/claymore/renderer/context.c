#include "context.h"

#include "GL/gl.h"
#include "GLFW/glfw3.h"

#include "claymore/debug/debug.h"
#include "claymore/logger/logger.h"

bool cm_context_init(WindowHandle *window_handle) {

  /* Make the window's context current */
  glfwMakeContextCurrent(window_handle);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    CM_ERROR("GLEW initialization failed: %s\n", glewGetString(err));
    return false;
  }

  glEnable(GL_DEPTH_TEST);

  // Transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  const struct color {
    float red;
    float green;
    float blue;
  } bg = {0.1F, 0.1F, 0.1F};
  glClearColor(bg.red, bg.green, bg.blue, 1.0F);

#ifdef _CM_DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback((GLDEBUGPROC)cm_debug_message_callback, 0);
#endif

  return true;
}
