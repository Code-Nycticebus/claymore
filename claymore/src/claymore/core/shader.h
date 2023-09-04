#ifndef __CM_SHADER_H__
#define __CM_SHADER_H__

#include "cm.h"

typedef struct {
  GLint id;
} CmShader;

// TODO rename to fit the naming schema: cm_shader_load...
CmShader cm_load_shader_from_file(const char *vs_file, const char *fs_file);
CmShader cm_load_shader_from_memory(const char *vs_src, const char *fs_src);

void cm_shader_delete(CmShader *shader);

void cm_shader_bind(const CmShader *shader);
GLint cm_shader_get_uniform_location(const CmShader *shader,
                                     const char *uniform_name);
void cm_shader_set_mat4(CmShader *shader, const char *u_name, const mat4 mat);
void cm_shader_set_f32(CmShader *shader, const char *u_name, float value);
void cm_shader_set_i32(CmShader *shader, const char *u_name, int32_t value);
#endif // __CM_SHADER_H__
