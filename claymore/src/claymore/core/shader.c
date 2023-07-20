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
    cm_log_err("[CLAYMORE][SHADER][FILE] %s\n", strerror(errno));
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  const size_t size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *data = calloc(sizeof(char), size + 1);
  if (data == NULL) {
    return NULL;
  }

  fread(data, sizeof(char), size, file);

  return data;
}

bool _cm_shader_check_error(GLuint shader_id, GLenum gl_check) {
  GLint result = GL_FALSE;
  GLsizei lenght = 0;
  glGetShaderiv(shader_id, gl_check, &result);
  glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &lenght);
  if (lenght > 0) {
    char *err_msg = calloc(sizeof(char), lenght);
    glGetShaderInfoLog(shader_id, lenght, NULL, err_msg);
    cm_log_err("%s\n%s\n", "[CLAYMORE][SHADER][COMPILE] %s\n", err_msg);
    free(err_msg);
    return false;
  }
  return true;
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
    cm_log_err("%s\n%s\n", vs_file, fs_file);
    return 0;
  }

  return program;
}

GLuint cm_load_shader_from_memory(const char *vs_src, const char *fs_src) {

  GLuint vs_id = _cm_compile_shader(vs_src, GL_VERTEX_SHADER);
  if (!_cm_shader_check_error(vs_id, GL_COMPILE_STATUS)) {
    cm_log_err("%s\n", vs_src);
    return 0;
  }

  GLuint fs_id = _cm_compile_shader(fs_src, GL_FRAGMENT_SHADER);
  if (!_cm_shader_check_error(fs_id, GL_COMPILE_STATUS)) {
    cm_log_err("%s\n", fs_src);
  }

  GLuint program = glCreateProgram();

  glAttachShader(program, vs_id);
  glAttachShader(program, fs_id);
  glLinkProgram(program);

  glDeleteShader(vs_id);
  glDeleteShader(fs_id);

  GLint result = GL_FALSE;
  GLsizei lenght = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &result);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &lenght);
  if (lenght > 0) {
    char *err_msg = calloc(sizeof(char), lenght);
    glGetShaderInfoLog(program, lenght, NULL, err_msg);
    free(err_msg);

    cm_log_err("[CLAYMORE][SHADER][LINK] %s\n", err_msg);
    return 0;
  }

  return program;
}

GLint cm_shader_get_uniform_location(GLuint shader, const char *uniform_name) {
  GLint location = glGetUniformLocation(shader, uniform_name);
  if (location == -1) {
    cm_log_err("Uniform location '%s' not found in shader %u\n", uniform_name,
               shader);
  }
  return location;
}
