#include "debug.h"
#include "claymore/logger/logger.h"

#ifdef _CM_DEBUG

const bool cm_debug_enabled = true;

#include <stdio.h>

void cm_debug_message_callback(GLenum source, GLenum type, GLuint id,
                               GLenum severity, GLsizei length,
                               const GLchar *message, const void *userParam) {
  (void)source;
  (void)id;
  (void)length;
  (void)userParam;
  CM_ERROR("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
             (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type,
             severity, message);
}
#else

const bool cm_debug_enabled = false;

#endif
