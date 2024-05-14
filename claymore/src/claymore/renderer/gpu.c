#include "gpu.h"

#include <GL/glew.h>

static GLenum get_type(CmGpuType type) {
  switch (type) {
  case CM_STATIC_DRAW:
    return GL_STATIC_DRAW;
  case CM_DYNAMIC_DRAW:
    return GL_DYNAMIC_DRAW;
  }
  cebus_assert(false, "UNREACHABLE");
  return 0;
}

static GLenum get_mode(CmGpuDrawMode mode) {
  switch (mode) {
  case CM_DRAW_TRIANGLES:
    return GL_TRIANGLES;
  case CM_DRAW_LINES:
    return GL_LINES;
  }
  cebus_assert(false, "UNREACHABLE");
  return 0;
}

CmVbo cm_gpu_vbo(CmGpu *b, CmGpuType type, usize s, usize len, const float *v) {
  CmVbo vbo = {.len = len};

  glGenBuffers(1, &vbo.id);
  glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
  glBufferData(GL_ARRAY_BUFFER, len * s, v, get_type(type));

  da_push(&b->vbo, vbo.id);
  return vbo;
}

void cm_gpu_vbo_update(CmVbo *vbo, usize s, usize len, const float *v) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
  glBufferSubData(GL_ARRAY_BUFFER, 0, len * s, v);
  vbo->len = len;
}

void cm_gpu_vbo_draw_instanced(CmVbo *vbo, usize count, CmGpuDrawMode mode) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
  glDrawArraysInstanced(get_mode(mode), 0, vbo->len, count);
}

CmEbo cm_gpu_ebo(CmGpu *b, CmGpuType type, usize count, const u32 *i) {
  CmEbo ebo = {.count = count};

  glGenBuffers(1, &ebo.id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), i, get_type(type));

  da_push(&b->ebo, ebo.id);
  return ebo;
}

void cm_gpu_ebo_draw_instanced(CmEbo *ebo, usize count, CmGpuDrawMode mode) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->id);
  glDrawElementsInstanced(get_mode(mode), ebo->count, GL_UNSIGNED_INT, NULL,
                          count);
}

void cm_gpu_ebo_draw(CmEbo *ebo, usize count, CmGpuDrawMode mode) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->id);
  glDrawElements(get_mode(mode), count, GL_UNSIGNED_INT, NULL);
}

CmVao cm_gpu_vao(CmGpu *b) {
  CmVao vao = {0};
  glGenVertexArrays(1, &vao.id);
  glBindVertexArray(vao.id);

  da_push(&b->vao, vao.id);

  return vao;
}

void cm_gpu_vao_bind(CmVao *vao) { glBindVertexArray(vao->id); }

void cm_gpu_vao_push(CmVao *vao, usize count, usize stride, usize offset) {
  glBindVertexArray(vao->id);
  glEnableVertexAttribArray(vao->idx);
  glVertexAttribPointer(vao->idx, count, GL_FLOAT, GL_FALSE, stride,
                        (void *)offset);
  vao->idx++;
}

void cm_gpu_vao_instanced(CmVao *vao, usize instance, usize count, usize stride,
                          usize offset) {
  glBindVertexArray(vao->id);
  glEnableVertexAttribArray(vao->idx);
  glVertexAttribPointer(vao->idx, count, GL_FLOAT, GL_FALSE, stride,
                        (void *)offset);
  glVertexAttribDivisor(vao->idx, instance);
  vao->idx++;
}

CmGpuID cm_gpu_program(CmGpu *b) {
  CmGpuID program = glCreateProgram();
  da_push(&b->program, program);
  return program;
}

CmGpu cm_gpu_internal_init(Arena *arena) {
  CmGpu gpu = {0};
  gpu.arena = arena;
  da_init(&gpu.vbo, arena);
  da_init(&gpu.vao, arena);
  da_init(&gpu.ebo, arena);
  da_init(&gpu.program, arena);
  return gpu;
}

void cm_gpu_internal_free(CmGpu *b) {
  glDeleteVertexArrays(b->vao.len, b->vao.items);
  glDeleteBuffers(b->ebo.len, b->ebo.items);
  glDeleteBuffers(b->vbo.len, b->vbo.items);
  for (usize i = 0; i < b->program.len; ++i) {
    glDeleteProgram(b->program.items[i]);
  }
}
