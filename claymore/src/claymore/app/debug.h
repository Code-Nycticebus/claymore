#ifndef __CLAYMORE_DEBUG_H__
#define __CLAYMORE_DEBUG_H__

#include "claymore/defines.h"

// For runtime checks
extern const bool cm_debug_enabled;

#ifdef _CM_DEBUG
#include <GL/glew.h>
void GLEWAPIENTRY cm_debug_message_callback(GLenum source, GLenum type,
                                            GLuint id, GLenum severity,
                                            GLsizei length,
                                            const GLchar *message,
                                            const void *userParam);
#endif // _CM_DEBUG

#endif /* !__CLAYMORE_DEBUG_H__ */
