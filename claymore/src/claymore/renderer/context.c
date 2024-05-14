#include "context.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef CLAYMORE_DEBUG
static void APIENTRY cm_debug_message_callback(GLenum source, GLenum type,
                                               GLuint id, GLenum severity,
                                               GLsizei length,
                                               const GLchar *message,
                                               const void *userParam) {
  const char *_source;
  const char *_type;
  (void)length, (void)userParam;

  switch (source) {
  case GL_DEBUG_SOURCE_API:
    _source = "API";
    break;

  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    _source = "WINDOW SYSTEM";
    break;

  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    _source = "SHADER COMPILER";
    break;

  case GL_DEBUG_SOURCE_THIRD_PARTY:
    _source = "THIRD PARTY";
    break;

  case GL_DEBUG_SOURCE_APPLICATION:
    _source = "APPLICATION";
    break;

  case GL_DEBUG_SOURCE_OTHER:
    _source = "UNKNOWN";
    break;

  default:
    _source = "UNKNOWN";
    break;
  }

  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    _type = "ERROR";
    break;

  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    _type = "DEPRECATED BEHAVIOR";
    break;

  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    _type = "UNDEFINED BEHAVIOR";
    break;

  case GL_DEBUG_TYPE_PORTABILITY:
    _type = "PORTABILITY";
    break;

  case GL_DEBUG_TYPE_PERFORMANCE:
    _type = "PERFORMANCE";
    break;

  case GL_DEBUG_TYPE_OTHER:
    _type = "OTHER";
    break;

  case GL_DEBUG_TYPE_MARKER:
    _type = "MARKER";
    break;

  default:
    _type = "UNKNOWN";
    break;
  }

  switch (severity) {
  default:
    // Ignore
    break;
  case GL_DEBUG_SEVERITY_LOW:
    cebus_log_info("CALLBACK: %s(%d) %s: %s", _type, id, _source, message);
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    cebus_log_warning("CALLBACK: %s(%d) %s: %s", _type, id, _source, message);
    break;
  case GL_DEBUG_SEVERITY_HIGH:
    cebus_log_error("CALLBACK: %s(%d) %s: %s", _type, id, _source, message);
    DEBUGBREAK();
  }
}
#endif

bool cm_platform_context_init(void *window_context) {
  glfwMakeContextCurrent(window_context);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    cebus_log_error("GLEW initialization failed: %s", glewGetString(err));
    return false;
  }

  /* Default OpenGl Options */
  // glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND); // Enables Transparency of png files
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glfwSwapInterval(0);

  int32_t width;
  int32_t height;
  glfwGetWindowSize(window_context, &width, &height);
  glViewport(0, 0, width, height);

#ifdef CLAYMORE_DEBUG
  // Logs info
  cebus_log_info("OpenGl %s", glGetString(GL_VERSION));
  cebus_log_info("GPU: %s", glGetString(GL_RENDERER));
  cebus_log_info("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
  // Debug
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback((GLDEBUGPROC)cm_debug_message_callback, NULL);
#endif

  return true;
}
