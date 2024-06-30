#include "gpu.h"

#include <glad.h>

/* ========= gpu public ========= */

CmVbo cm_gpu_vbo(CmGpu *b, CmGpuType type, usize s, usize len, const float *v) {
  CmVbo vbo = {.len = len};

  glGenBuffers(1, &vbo.id);
  glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
  glBufferData(GL_ARRAY_BUFFER, len * s, v, type);

  da_push(&b->buffers, (CmGpuBuffer){.type = CM_GPU_VBO, .id = vbo.id});
  return vbo;
}

void cm_gpu_vbo_update(CmVbo *vbo, usize s, usize len, const float *v) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
  glBufferSubData(GL_ARRAY_BUFFER, 0, len * s, v);
  vbo->len = len;
}

void cm_gpu_vbo_draw(CmVbo *vbo, CmGpuDrawMode mode) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
  glDrawArrays(mode, 0, vbo->len);
}

void cm_gpu_vbo_draw_instanced(CmVbo *vbo, usize count, CmGpuDrawMode mode) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
  // This could be wrong idk
  glDrawArraysInstanced(mode, 0, count, vbo->len);
}

CmEbo cm_gpu_ebo(CmGpu *b, CmGpuType type, usize count, const u32 *i) {
  CmEbo ebo = {.count = count};

  glGenBuffers(1, &ebo.id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), i, type);

  da_push(&b->buffers, (CmGpuBuffer){.type = CM_GPU_EBO, .id = ebo.id});
  return ebo;
}

void cm_gpu_ebo_draw_instanced(CmEbo *ebo, usize count, CmGpuDrawMode mode) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->id);
  glDrawElementsInstanced(mode, ebo->count, GL_UNSIGNED_INT, NULL, count);
}

void cm_gpu_ebo_draw(CmEbo *ebo, usize count, CmGpuDrawMode mode) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->id);
  glDrawElements(mode, count, GL_UNSIGNED_INT, NULL);
}

CmVao cm_gpu_vao(CmGpu *b) {
  CmVao vao = {0};
  glGenVertexArrays(1, &vao.id);
  glBindVertexArray(vao.id);

  da_push(&b->buffers, (CmGpuBuffer){.type = CM_GPU_VAO, .id = vao.id});

  return vao;
}

void cm_gpu_vao_bind(CmVao *vao) { glBindVertexArray(vao->id); }

void cm_gpu_vao_push(CmVao *vao, usize count, usize stride, usize offset) {
  glBindVertexArray(vao->id);
  glEnableVertexAttribArray(vao->idx);
  glVertexAttribPointer(vao->idx, count, GL_FLOAT, GL_FALSE, stride,
                        (void *)offset); // NOLINT
  vao->idx++;
}

void cm_gpu_vao_instanced(CmVao *vao, usize instance, usize count, usize stride,
                          usize offset) {
  glBindVertexArray(vao->id);
  glEnableVertexAttribArray(vao->idx);
  glVertexAttribPointer(vao->idx, count, GL_FLOAT, GL_FALSE, stride,
                        (void *)offset); // NOLINT
  glVertexAttribDivisor(vao->idx, instance);
  vao->idx++;
}

CmGpuID cm_gpu_program(CmGpu *b) {
  CmGpuID program = glCreateProgram();
  da_push(&b->buffers, (CmGpuBuffer){.type = CM_GPU_PROGRAM, .id = program});
  return program;
}

CmGpuID cm_gpu_texture(CmGpu *b) {
  CmGpuID texture = glCreateProgram();
  glGenTextures(1, &texture);
  da_push(&b->buffers, (CmGpuBuffer){.type = CM_GPU_TEXTURE, .id = texture});
  return texture;
}

/* ========= gpu internal ========= */

CmGpu cm_gpu_internal_init(Arena *arena) {
  CmGpu gpu = {0};
  gpu.arena = arena;
  da_init(&gpu.buffers, arena);
  return gpu;
}

void cm_gpu_internal_free(CmGpu *b) {
  for (usize i = 0; i < da_len(&b->buffers); ++i) {
    CmGpuBuffer *buffer = &da_get(&b->buffers, i);
    switch (buffer->type) {
    case CM_GPU_EBO:
    case CM_GPU_VBO: {
      glDeleteBuffers(1, &buffer->id);
    } break;
    case CM_GPU_VAO: {
      glDeleteVertexArrays(1, &buffer->id);
    } break;
    case CM_GPU_PROGRAM: {
      glDeleteProgram(buffer->id);
    } break;
    case CM_GPU_TEXTURE: {
      glDeleteTextures(1, &buffer->id);
    } break;
    }
  }
}
