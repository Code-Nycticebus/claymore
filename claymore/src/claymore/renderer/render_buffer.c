#include "render_buffer.h"
#include "render_command.h"

CmVertexArray cm_vertex_array_create(void) {
  CmVertexArray va = {0};
  glGenVertexArrays(1, &va.id);
  return va;
}
void cm_vertex_array_delete(CmVertexArray *vao) {
  glDeleteVertexArrays(1, &vao->id);
  *vao = (CmVertexArray){0};
}

void cm_vertex_array_push_attrib(CmVertexArray *array, size_t count,
                                 size_t stride, size_t offset) {
  glBindVertexArray(array->id);
  glEnableVertexAttribArray(array->index);
  glVertexAttribPointer(array->index, count, GL_FLOAT, GL_FALSE, stride,
                        (void *)offset); // NOLINT
  array->index++;
}

void cm_vertex_array_push_attrib_instanced(CmVertexArray *array, size_t count,
                                           size_t stride, size_t offset) {
  glBindVertexArray(array->id);
  glEnableVertexAttribArray(array->index);
  glVertexAttribPointer(array->index, count, GL_FLOAT, GL_FALSE, stride,
                        (void *)offset); // NOLINT
  glVertexAttribDivisor(array->index, 1);
  array->index++;
}

CmIndexBuffer cm_index_buffer_create(const uint32_t *indices, size_t count) {
  CmIndexBuffer ib = {0};
  glGenBuffers(1, &ib.id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices,
               GL_STATIC_DRAW);
  ib.count = count;
  return ib;
}

void cm_index_buffer_delete(CmIndexBuffer *ibo) {
  glDeleteBuffers(1, &ibo->id);
  *ibo = (CmIndexBuffer){0};
}

CmVertexBuffer cm_vertex_buffer_create(const void *data, size_t bytes,
                                       CmBufferUsage buffer_usage) {
  CmVertexBuffer vb = {0};
  glGenBuffers(1, &vb.id);
  glBindBuffer(GL_ARRAY_BUFFER, vb.id);
  glBufferData(GL_ARRAY_BUFFER, bytes, data, buffer_usage);
  vb.size = bytes;
  vb.usage = buffer_usage;
  return vb;
}

void cm_vertex_buffer_delete(CmVertexBuffer *vbo) {
  glDeleteBuffers(1, &vbo->id);
  *vbo = (CmVertexBuffer){0};
}

CmVertexBuffer *cm_buffer_attach_vec2(CmRenderBuffer *buffer, const vec2s *data,
                                      size_t count,
                                      CmBufferUsage buffer_usage) {
  assert(buffer->vbo_count < CM_RENDER_BUFFER_MAX);
  CmVertexBuffer *vb = &buffer->vbo[buffer->vbo_count];
  *vb = cm_vertex_buffer_create(data, sizeof(vec2s) * count, buffer_usage);
  if (buffer_usage == CM_BUFFER_INSTANCED) {
    cm_vertex_array_push_attrib_instanced(&buffer->vao, 3, sizeof(vec2s), 0);
    buffer->instance_count = count;
  } else {
    cm_vertex_array_push_attrib(&buffer->vao, 2, sizeof(vec2s), 0);
  }
  buffer->vbo_count++;
  return vb;
}

void cm_buffer_update_vec2(CmRenderBuffer *buffer, CmVertexBuffer *vb,
                           const vec2s *data, size_t count) {
  glBindBuffer(GL_ARRAY_BUFFER, vb->id);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec2s) * count, data);
  buffer->instance_count = count;
}

CmVertexBuffer *cm_buffer_attach_vec3(CmRenderBuffer *buffer, const vec3s *data,
                                      size_t count,
                                      CmBufferUsage buffer_usage) {
  assert(buffer->vbo_count < CM_RENDER_BUFFER_MAX);
  CmVertexBuffer *vb = &buffer->vbo[buffer->vbo_count];
  *vb = cm_vertex_buffer_create(data, sizeof(vec3s) * count, buffer_usage);
  if (buffer_usage == CM_BUFFER_INSTANCED) {
    cm_vertex_array_push_attrib_instanced(&buffer->vao, 3, sizeof(vec3s), 0);
    buffer->instance_count = count;
  } else {
    cm_vertex_array_push_attrib(&buffer->vao, 3, sizeof(vec3s), 0);
  }
  buffer->vbo_count++;
  return vb;
}

void cm_buffer_update_vec3(CmRenderBuffer *buffer, CmVertexBuffer *vb,
                           const vec3s *data, size_t count) {
  glBindBuffer(GL_ARRAY_BUFFER, vb->id);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3s) * count, data);
  buffer->instance_count = count;
}

CmVertexBuffer *cm_buffer_attach_vec4(CmRenderBuffer *buffer, const vec2s *data,
                                      size_t count,
                                      CmBufferUsage buffer_usage) {
  assert(buffer->vbo_count < CM_RENDER_BUFFER_MAX);
  CmVertexBuffer *vb = &buffer->vbo[buffer->vbo_count];
  *vb = cm_vertex_buffer_create(data, sizeof(vec4s) * count, buffer_usage);
  if (buffer_usage == CM_BUFFER_INSTANCED) {
    cm_vertex_array_push_attrib_instanced(&buffer->vao, 3, sizeof(vec4s), 0);
    buffer->instance_count = count;
  } else {
    cm_vertex_array_push_attrib(&buffer->vao, 4, sizeof(vec4s), 0);
  }
  buffer->vbo_count++;
  return vb;
}

void cm_buffer_update_vec4(CmRenderBuffer *buffer, CmVertexBuffer *vb,
                           const vec4s *data, size_t count) {
  glBindBuffer(GL_ARRAY_BUFFER, vb->id);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4s) * count, data);
  buffer->instance_count = count;
}

CmVertexBuffer *cm_buffer_attach_mat4(CmRenderBuffer *buffer, const mat4s *data,
                                      size_t count) {
  assert(buffer->vbo_count < CM_RENDER_BUFFER_MAX);
  buffer->instance_count = count;
  CmVertexBuffer *vb = &buffer->vbo[buffer->vbo_count];
  *vb = cm_vertex_buffer_create(data, sizeof(mat4s) * count, CM_BUFFER_DYNAMIC);
  for (size_t i = 0; i < 4; i++) {
    cm_vertex_array_push_attrib_instanced(&buffer->vao, 4, sizeof(mat4s),
                                          sizeof(vec4s) * i);
  }

  buffer->vbo_count++;
  return vb;
}

void cm_buffer_update_mat4(CmRenderBuffer *buffer, CmVertexBuffer *vb,
                           const mat4s *data, size_t count) {
  glBindBuffer(GL_ARRAY_BUFFER, vb->id);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(mat4s) * count, data);
  buffer->instance_count = count;
}

CmRenderBuffer cm_render_buffer_create(const vec3s *positions, size_t count,
                                       const uint32_t *indices,
                                       size_t indices_count) {
  CmRenderBuffer buffer = {0};
  buffer.instance_count = 1;
  cm_buffer_attach_vec3(&buffer, positions, count, CM_BUFFER_STATIC);
  buffer.ibo = cm_index_buffer_create(indices, indices_count);
  return buffer;
}

void cm_render_buffer_delete(CmRenderBuffer *buffer) {
  cm_vertex_array_delete(&buffer->vao);
  for (size_t i = 0; i < buffer->vbo_count; i++) {
    cm_vertex_buffer_delete(&buffer->vbo[i]);
  }
  cm_index_buffer_delete(&buffer->ibo);
}

void cm_buffer_draw(CmRenderBuffer *buffer) {
  glBindVertexArray(buffer->vao.id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ibo.id);
  glDrawElementsInstanced(GL_TRIANGLES, buffer->ibo.count, GL_UNSIGNED_INT,
                          NULL, buffer->instance_count);
}

CmFrameBuffer cm_framebuffer_create(uint32_t width, uint32_t height) {
  CmFrameBuffer fb;

  /* Frame buffer */
  glGenFramebuffers(1, &fb.fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fb.fbo);

  // Texture to render the framebuffer to
  glGenTextures(1, &fb.texture);
  glBindTexture(GL_TEXTURE_2D, fb.texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);

  // Specify the attachment if not color attachment.
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         fb.texture, 0);

  // Render buffer object
  glGenRenderbuffers(1, &fb.rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, fb.rbo);

  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

  // Attach the renderbuffer to framebuffer
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, fb.rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    cm_log_error("Frame buffer was not initialized correctly!\n");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return (CmFrameBuffer){0};
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return fb;
}

void cm_framebuffer_bind(CmFrameBuffer *framebuffer) {
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo);
  glClear(GL_COLOR_BUFFER_BIT |
          GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
}

void cm_framebuffer_unbind(void) { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void cm_framebuffer_draw(CmFrameBuffer *framebuffer, CmRenderBuffer *rb) {
  glDisable(GL_DEPTH_TEST);
  glBindTexture(GL_TEXTURE_2D, framebuffer->texture);

  cm_buffer_draw(rb);

  glBindTexture(GL_TEXTURE_2D, 0);
  glEnable(GL_DEPTH_TEST);
}

void cm_framebuffer_delete(CmFrameBuffer *framebuffer) {
  glDeleteFramebuffers(1, &framebuffer->fbo);
  glDeleteRenderbuffers(1, &framebuffer->rbo);
  glDeleteTextures(1, &framebuffer->texture);
}
