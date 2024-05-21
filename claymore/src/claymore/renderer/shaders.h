#ifndef __CLAYMORE_SHADERS_H__
#define __CLAYMORE_SHADERS_H__

#include "claymore/defines.h" // IWYU pragma: export
#include "claymore/renderer/gpu.h"

#ifndef CM_SHADER_UNIFORM_MAX
#define CM_SHADER_UNIFORM_MAX 10
#endif

typedef struct {
  u32 id;
  usize uniform_count;
  struct {
    u64 hash;
    i32 location;
  } uniforms[CM_SHADER_UNIFORM_MAX];
} CmShader;

CmShader cm_shader_from_file(CmGpu *gpu, Str vs, Str fs, Error *e);
CmShader cm_shader_from_memory(CmGpu *gpu, Str vs, Str fs, Error *e);

void cm_shader_bind(const CmShader *shader);
void cm_shader_unbind(void);

void cm_shader_set_mat4(CmShader *shader, Str uniform_name, mat4 mat);
void cm_shader_set_vec2(CmShader *shader, Str uniform_name, vec2 vec);
void cm_shader_set_vec3(CmShader *shader, Str uniform_name, vec3 vec);
void cm_shader_set_vec4(CmShader *shader, Str uniform_name, vec4 vec);
void cm_shader_set_f32(CmShader *shader, Str uniform_name, f32 value);
void cm_shader_set_i32(CmShader *shader, Str uniform_name, i32 value);

#endif /* !__CLAYMORE_SHADERS_H__ */
