#include "shaders.h"

#include <GL/glew.h>

#include <stdio.h>

#define _cm_check_shader(error, shader_id, gl_check, gl_get_iv, gl_get_log)    \
  {                                                                            \
    GLint result = GL_FALSE;                                                   \
    GLsizei lenght = 0;                                                        \
    gl_get_iv(shader_id, gl_check, &result);                                   \
    gl_get_iv(shader_id, GL_INFO_LOG_LENGTH, &lenght);                         \
    if (lenght > 0) {                                                          \
      char *err_msg = calloc(sizeof(char), lenght);                            \
      gl_get_log(shader_id, lenght, NULL, err_msg);                            \
      error_emit(error, -1, "%s", err_msg);                                    \
      free(err_msg);                                                           \
      return false;                                                            \
    }                                                                          \
    return true;                                                               \
  }

static bool _cm_shader_check_error(GLuint shader_id, GLenum gl_check,
                                   Error *error) {
  if (gl_check == GL_COMPILE_STATUS) {
    _cm_check_shader(error, shader_id, gl_check, glGetShaderiv,
                     glGetShaderInfoLog);
  }
  if (gl_check == GL_LINK_STATUS) {
    _cm_check_shader(error, shader_id, gl_check, glGetProgramiv,
                     glGetProgramInfoLog);
  }

  clib_assert(gl_check == GL_COMPILE_STATUS || gl_check == GL_LINK_STATUS,
              "Other checks are not supported");
  return false;
}

#undef _cm_check_shader

static GLuint _cm_compile_shader(Str shader_src, GLenum type, Error *error) {
  clib_assert((type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER),
              "Shader only supports these types for now!");

  GLuint shader_id = glCreateShader(type);

  const GLint len = shader_src.len;
  glShaderSource(shader_id, 1, &shader_src.data, &len);
  glCompileShader(shader_id);

  _cm_shader_check_error(shader_id, GL_COMPILE_STATUS, error);
  error_propagate(error, { error_add_note(STR_FMT, STR_ARG(shader_src)); });

  return shader_id;
}

CmShader *cm_shader_load_from_file(Arena *arena, Str vs, Str fs, Error *e) {
  CmShader *program = NULL;
  Arena temp = {0};

  Str vs_content = file_read_str(vs, &temp, e);
  error_propagate(e, { goto defer; });

  Str fs_content = file_read_str(fs, &temp, e);
  error_propagate(e, { goto defer; });

  program = cm_shader_load_from_memory(arena, vs_content, fs_content, e);
  error_propagate(e, { goto defer; });

defer:
  arena_free(&temp);
  return program;
}

CmShader *cm_shader_load_from_memory(Arena *arena, Str vs, Str fs, Error *e) {
  CmShader *program = NULL;
  GLuint vs_id = 0;
  GLuint fs_id = 0;

  vs_id = _cm_compile_shader(vs, GL_VERTEX_SHADER, e);
  error_propagate(e, { goto defer; });

  fs_id = _cm_compile_shader(fs, GL_FRAGMENT_SHADER, e);
  error_propagate(e, { goto defer; });

  CmShader p = {0};
  p.id = glCreateProgram();

  glAttachShader(p.id, vs_id);
  glAttachShader(p.id, fs_id);
  glLinkProgram(p.id);

  _cm_shader_check_error(p.id, GL_LINK_STATUS, e);
  error_propagate(e, { goto defer; });

  program = arena_calloc(arena, sizeof(CmShader));
  *program = p;
defer:
  if (vs_id) {
    glDeleteShader(vs_id);
  }
  if (fs_id) {
    glDeleteShader(fs_id);
  }
  return program;
}

void cm_shader_delete(CmShader *shader) { glDeleteProgram(shader->id); }

void cm_shader_bind(const CmShader *shader) { glUseProgram(shader->id); }
void cm_shader_unbind(void) { glUseProgram(0); }

static i32 _cm_shader_get_uniform_location(CmShader *shader, Str uniform_name) {
  u64 hash = str_hash(uniform_name);
  glUseProgram(shader->id);
  for (size_t i = 0; i < shader->uniform_count; i++) {
    if (shader->uniforms[i].hash == hash) {
      return shader->uniforms[i].location;
    }
  }

  Arena temp = {0};

  Str name = str_copy(uniform_name, &temp);
  GLint location = glGetUniformLocation(shader->id, name.data);
  if (location == -1) {
    clib_log_error("Uniform location '" STR_FMT "' not found in shader %u",
                   STR_ARG(uniform_name), shader->id);
  }

  clib_assert(shader->uniform_count < CM_SHADER_UNIFORM_MAX,
              "shader has to many cached uniforms");

  shader->uniforms[shader->uniform_count].location = location;
  shader->uniforms[shader->uniform_count].hash = hash;
  shader->uniform_count++;

  arena_free(&temp);
  return location;
}

void cm_shader_set_mat4(CmShader *shader, Str uniform_name, const mat4 mat) {
  i32 location = _cm_shader_get_uniform_location(shader, uniform_name);
  glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

void cm_shader_set_vec3(CmShader *shader, Str uniform_name, const vec3 vec) {
  i32 location = _cm_shader_get_uniform_location(shader, uniform_name);
  glUniform3fv(location, 1, vec);
}

void cm_shader_set_vec4(CmShader *shader, Str uniform_name, const vec4 vec) {
  i32 location = _cm_shader_get_uniform_location(shader, uniform_name);
  glUniform4fv(location, 1, vec);
}

void cm_shader_set_f32(CmShader *shader, Str uniform_name, float value) {
  i32 location = _cm_shader_get_uniform_location(shader, uniform_name);
  glUniform1f(location, value);
}

void cm_shader_set_i32(CmShader *shader, Str uniform_name, int32_t value) {
  i32 location = _cm_shader_get_uniform_location(shader, uniform_name);
  glUniform1i(location, value);
}
