#include "gpu.h"

#include <GL/glew.h>

static GLenum get_type(CmGpuType type) {
  switch (type) {
  case CM_STATIC_DRAW:
    return GL_STATIC_DRAW;
  case CM_DYNAMIC_DRAW:
    return GL_DYNAMIC_DRAW;
  }
  clib_assert(false, "UNREACHABLE");
  return 0;
}

CmGpu cm_gpu_internal_init(Arena *arena) {
  CmGpu gpu = {0};
  da_init(&gpu.vbo, arena);
  da_init(&gpu.vao, arena);
  da_init(&gpu.ebo, arena);
  return gpu;
}
void cm_gpu_internal_free(CmGpu *b) {
  glDeleteVertexArrays(b->vao.len, b->vao.items);
  glDeleteBuffers(b->ebo.len, b->ebo.items);
  glDeleteBuffers(b->vbo.len, b->vbo.items);
}

u32 cm_gpu_vbo(CmGpu *b, CmGpuType type, usize size, const float *v) {
  u32 vbo;

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, size, v, get_type(type));

  da_push(&b->vbo, vbo);
  return vbo;
}

void cm_gpu_update_vbo(u32 buffer, usize size, const float *v) {
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, v);
}

u32 cm_gpu_ebo(CmGpu *b, CmGpuType type, usize count, const u32 *i) {
  u32 ebo;

  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), i, get_type(type));

  da_push(&b->ebo, ebo);
  return ebo;
}

u32 cm_gpu_vao(CmGpu *b) {
  u32 vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  da_push(&b->vao, vao);

  return vao;
}
