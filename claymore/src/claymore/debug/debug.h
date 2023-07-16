#ifndef __CM_DEBUG_H__
#define __CM_DEBUG_H__

#include <stdbool.h>

// For runtime checks
extern const bool cm_debug_enabled;

#ifdef _CM_DEBUG

#include "GL/glew.h"
void cm_debug_message_callback(GLenum source, GLenum type, GLuint id,
                               GLenum severity, GLsizei length,
                               const GLchar *message, const void *userParam);

#else // !_CM_DEBUG

#endif // _CM_DEBUG

#endif /* !__CM_DEBUG_H__ */
