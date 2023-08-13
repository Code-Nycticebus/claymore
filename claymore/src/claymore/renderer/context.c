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
    cm_log_error("GLEW initialization failed: %s", glewGetString(err));
    return false;
  }
  cm_log_info("Glew version: %s\n", glewGetString(GLEW_VERSION));
  cm_log_info("OpenGl %s\n", glGetString(GL_VERSION));
  cm_log_info("GPU: %s\n", glGetString(GL_RENDERER));
  cm_log_info("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  // Transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  int32_t width;
  int32_t height;
  glfwGetWindowSize(window_handle, &width, &height);
  glViewport(0, 0, width, height);

#ifdef _CM_DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback((GLDEBUGPROC)cm_debug_message_callback, 0);
#endif

  return true;
}
