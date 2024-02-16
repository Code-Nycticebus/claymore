#include "buffer.h"

#include "GL/glew.h"

static GLenum get_type(CmBufferType type) {
  switch (type) {
  case CM_STATIC_DRAW:
    return GL_STATIC_DRAW;
  case CM_DYNAMIC_DRAW:
    return GL_DYNAMIC_DRAW;
  }
  clib_assert(false, "UNREACHABLE");
  return 0;
}

CmBuffers cm_buffer_init(Arena *arena) {
  CmBuffers buffers = {0};
  da_init(&buffers.vbo, arena);
  da_init(&buffers.vao, arena);
  da_init(&buffers.ebo, arena);
  return buffers;
}
void cm_buffer_free(CmBuffers *b) {
  glDeleteVertexArrays(b->vao.len, b->vao.items);
  glDeleteBuffers(b->ebo.len, b->ebo.items);
  glDeleteBuffers(b->vbo.len, b->vbo.items);
}

u32 cm_buffer_vbo(CmBuffers *b, CmBufferType type, const float *v, usize size) {
  u32 vbo;

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, size, v, get_type(type));

  da_push(&b->vbo, vbo);
  return vbo;
}

u32 cm_buffer_ebo(CmBuffers *b, CmBufferType type, const u32 *i, usize count) {
  u32 ebo;

  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), i, get_type(type));

  da_push(&b->ebo, ebo);
  return ebo;
}

u32 cm_buffer_vao(CmBuffers *b) {
  u32 vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  da_push(&b->vao, vao);

  return vao;
}
