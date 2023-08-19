#include "shader.h"
#include "claymore/logger/logger.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *_cm_shader_slurp_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    cm_log_error("Could not open sader: %s: %s\n", filename, strerror(errno));
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  const size_t file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *data = calloc(sizeof(char), file_size + 1);
  if (data == NULL) {
    return NULL;
  }

  if (fread(data, sizeof(char), file_size, file) != file_size && ferror(file)) {
    cm_log_error("Could not read fully slurp the file: %s: %s\n", filename,
                 strerror(errno));
    return NULL;
  }

  return data;
}

#define _cm_check(shader_id, gl_check, gl_get_iv, gl_get_log)                  \
  {                                                                            \
    GLint result = GL_FALSE;                                                   \
    GLsizei lenght = 0;                                                        \
    gl_get_iv(shader_id, gl_check, &result);                                   \
    gl_get_iv(shader_id, GL_INFO_LOG_LENGTH, &lenght);                         \
    if (lenght > 0) {                                                          \
      char *err_msg = calloc(sizeof(char), lenght);                            \
      gl_get_log(shader_id, lenght, NULL, err_msg);                            \
      cm_log_error("%s", err_msg);                                             \
      free(err_msg);                                                           \
      return false;                                                            \
    }                                                                          \
    return true;                                                               \
  }

bool _cm_shader_check_error(GLuint shader_id, GLenum gl_check) {
  if (gl_check == GL_COMPILE_STATUS) {
    _cm_check(shader_id, gl_check, glGetShaderiv, glGetShaderInfoLog);
  }
  if (gl_check == GL_LINK_STATUS) {
    _cm_check(shader_id, gl_check, glGetProgramiv, glGetProgramInfoLog);
  }

  assert(gl_check == GL_COMPILE_STATUS || gl_check == GL_LINK_STATUS);
  return false;
}

static GLuint _cm_compile_shader(const char *shader_src, GLenum type) {
  assert((type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER) &&
         "Shader only supports this for now!");

  GLuint shader_id = glCreateShader(type);

  glShaderSource(shader_id, 1, &shader_src, NULL);
  glCompileShader(shader_id);
  return shader_id;
}

GLuint cm_load_shader_from_file(const char *vs_file, const char *fs_file) {
  char *vs_src = _cm_shader_slurp_file(vs_file);
  if (vs_src == NULL) {
    return 0;
  }
  char *fs_src = _cm_shader_slurp_file(fs_file);
  if (fs_src == NULL) {
    return 0;
  }

  GLuint program = cm_load_shader_from_memory(vs_src, fs_src);
  free(vs_src);
  free(fs_src);
  if (program == 0) {
    cm_log_error("Vertex Shader %s\n", vs_file);
    cm_log_error("Fragment Shader %s\n", fs_file);
    return 0;
  }

  return program;
}

GLuint cm_load_shader_from_memory(const char *vs_src, const char *fs_src) {
  GLuint vs_id = _cm_compile_shader(vs_src, GL_VERTEX_SHADER);
  if (!_cm_shader_check_error(vs_id, GL_COMPILE_STATUS)) {
    cm_log_error("Vertex Shader:\n%s\n", vs_src);
    return 0;
  }
  GLuint fs_id = _cm_compile_shader(fs_src, GL_FRAGMENT_SHADER);
  if (!_cm_shader_check_error(fs_id, GL_COMPILE_STATUS)) {
    cm_log_error("Fragment Shader:\n%s\n", fs_src);
    return 0;
  }

  GLuint program = glCreateProgram();

  glAttachShader(program, vs_id);
  glAttachShader(program, fs_id);
  glLinkProgram(program);

  glDeleteShader(vs_id);
  glDeleteShader(fs_id);

  if (!_cm_shader_check_error(program, GL_LINK_STATUS)) {
    cm_log_error("Vertex Shader: \n%s\n", vs_src);
    cm_log_error("Fragment Shader: \n%s\n", fs_src);
    return 0;
  }

  return program;
}

GLint cm_shader_get_uniform_location(GLuint shader, const char *uniform_name) {
  assert(shader != 0);
  GLint location = glGetUniformLocation(shader, uniform_name);
  if (location == -1) {
    cm_log_error("Uniform location '%s' not found in shader %u\n", uniform_name,
                 shader);
  }
  return location;
}
