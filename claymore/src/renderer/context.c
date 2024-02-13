#include "context.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "claymore/app/debug.h"

bool cm_platform_context_init(void *window_context) {
  glfwMakeContextCurrent(window_context);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    clib_log_error("GLEW initialization failed: %s", glewGetString(err));
    return false;
  }

  /* Logs info */
  clib_log_info("OpenGl %s", glGetString(GL_VERSION));
  clib_log_info("GPU: %s", glGetString(GL_RENDERER));
  clib_log_info("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

  /* Default OpenGl Options */
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND); // Enables Transparency of png files
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  int32_t width;
  int32_t height;
  glfwGetWindowSize(window_context, &width, &height);
  glViewport(0, 0, width, height);

#ifdef _CM_DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback((GLDEBUGPROC)cm_debug_message_callback, NULL);
#endif

  return true;
}
