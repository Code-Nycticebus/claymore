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
    CM_ERROR("GLEW initialization failed: %s", glewGetString(err));
    return false;
  }
  CM_INFO("Glew version: %s\n", glewGetString(GLEW_VERSION));
  CM_INFO("OpenGl %s\n", glGetString(GL_VERSION));
  CM_INFO("GPU: %s\n", glGetString(GL_RENDERER));
  CM_INFO("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  glEnable(GL_DEPTH_TEST);

  // Transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef _CM_DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback((GLDEBUGPROC)cm_debug_message_callback, 0);
#endif

  return true;
}
