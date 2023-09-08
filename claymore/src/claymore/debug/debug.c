#include "debug.h"
#include "claymore/logger/logger.h"

#ifdef CM_DEBUG

const bool cm_debug_enabled = true;

#include <stdio.h>
#include <stdlib.h>

void cm_debug_message_callback(GLenum source, GLenum type, GLuint id,
                               GLenum severity, GLsizei length,
                               const GLchar *message, const void *userParam) {

  char *_source;
  char *_type;
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
    _type = "UDEFINED BEHAVIOR";
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

  if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
    cm_log_info("CALLBACK: %s(%d) %s: %s\n", _type, id, _source, message);
  } else if (severity == GL_DEBUG_SEVERITY_LOW) {
    cm_log_warning("CALLBACK: %s(%d) %s: %s\n", _type, id, _source, message);
  } else if (severity == GL_DEBUG_SEVERITY_MEDIUM ||
             severity == GL_DEBUG_SEVERITY_HIGH) {
    cm_log_error("CALLBACK: %s(%d) %s: %s\n", _type, id, _source, message);
  }
}
#else

const bool cm_debug_enabled = false;

#endif
