#ifndef __CM_SHADER_H__
#define __CM_SHADER_H__

#include "cm.h"

#define CM_SHADER_UNIFORM_MAX 3

typedef struct {
  GLint id;
  size_t cached_uniform_count;
  struct {
    const char *u_name;
    GLint location;
  } uniform_cache[CM_SHADER_UNIFORM_MAX];
} CmShader;

CmShader cm_shader_load_from_file(const char *vs_file, const char *fs_file);
CmShader cm_shader_load_from_memory(const char *vs_src, const char *fs_src);

void cm_shader_delete(CmShader *shader);

void cm_shader_bind(const CmShader *shader);
void cm_shader_unbind(void);
GLint cm_shader_get_uniform_location(CmShader *shader, const char *u_name);
void cm_shader_set_mat4(CmShader *shader, const char *u_name, mat4s mat);
void cm_shader_set_f32(CmShader *shader, const char *u_name, float value);
void cm_shader_set_i32(CmShader *shader, const char *u_name, int32_t value);

#endif // __CM_SHADER_H__
