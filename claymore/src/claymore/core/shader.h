#ifndef __CM_SHADER_H__
#define __CM_SHADER_H__

#include "cm.h"

GLuint cm_load_shader_from_file(const char *vs_file, const char *fs_file);

GLuint cm_load_shader_from_memory(const char *vs_src, const char *fs_src);

GLint cm_shader_get_uniform_location(GLuint shader, const char *uniform_name);

#endif // __CM_SHADER_H__
