#include "render_buffer.h"
#include "render_command.h"

CmVertexBuffer cm_vertex_buffer_create(size_t count, size_t element_size,
                                       const void *data, GLenum draw_mode) {
  CmVertexBuffer vertex_buffer = {
      .id = 0,
      .element_count = count,
      .element_size = element_size,
  };
  glGenBuffers(1, &vertex_buffer.id);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer.id);
  glBufferData(GL_ARRAY_BUFFER, count * element_size, data, draw_mode);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return vertex_buffer;
}

void cm_vertex_buffer_delete(CmVertexBuffer *vertex_buffer) {
  glDeleteBuffers(1, &vertex_buffer->id);

  vertex_buffer->id = 0;
  vertex_buffer->element_count = 0;
  vertex_buffer->element_size = 0;
}

CmVertexAttribute cm_vertex_attribute_create(CmVertexBuffer *vertex_buffer) {
  CmVertexAttribute vertex_attribute = {
      .id = 0,
      .index = 0,
      .buffer = vertex_buffer,
  };
  glGenVertexArrays(1, &vertex_attribute.id);
  return vertex_attribute;
}

void cm_vertex_attribute_delete(CmVertexAttribute *vertex_attrib) {
  glDeleteVertexArrays(1, &vertex_attrib->id);

  vertex_attrib->id = 0;
  vertex_attrib->buffer = NULL;
  vertex_attrib->index = 0;
}

void cm_vertex_attribute_push(CmVertexAttribute *attr, size_t count,
                              GLenum type, size_t offset) {
  // Bind vertex_buffer to associate vertex_attribute with it.
  assert(attr->buffer->id);
  glBindBuffer(GL_ARRAY_BUFFER, attr->buffer->id);

  assert(attr->id);
  glBindVertexArray(attr->id);

  glEnableVertexAttribArray(attr->index);
  glVertexAttribPointer(attr->index, count, type, GL_FALSE,
                        attr->buffer->element_size,
                        (const void *)offset); // NOLINT

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  attr->index++;
}

CmIndexBuffer cm_index_buffer_create(CmVertexAttribute *attrib, size_t count,
                                     const uint32_t *indices, GLenum mode) {
  CmIndexBuffer index_buffer = {.id = 0, .count = count, .attrib = attrib};
  // Bind vertex_attrib to associate index buffer with it
  glBindVertexArray(attrib->id);

  glGenBuffers(1, &index_buffer.id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer.id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
               mode);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return index_buffer;
}

void cm_index_buffer_delete(CmIndexBuffer *index_buffer) {
  glDeleteBuffers(1, &index_buffer->id);
  index_buffer->id = 0;
  index_buffer->count = 0;
  index_buffer->attrib = NULL;
}

void cm_render_buffer_delete(CmRenderBuffer *render_buffer) {
  cm_vertex_buffer_delete(&render_buffer->vertex_buffer);
  cm_vertex_attribute_delete(&render_buffer->vertex_attribute);
  cm_index_buffer_delete(&render_buffer->index_buffer);
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

void cm_framebuffer_draw(CmFrameBuffer *framebuffer, CmRenderBuffer *rb,
                         CmShader *shader) {
  glDisable(GL_DEPTH_TEST);
  cm_shader_bind(shader);
  cm_shader_set_i32(shader, "u_texture", 0);
  glBindTexture(GL_TEXTURE_2D, framebuffer->texture);

  cm_renderer_draw_indexed(rb, rb->index_buffer.count);

  glBindTexture(GL_TEXTURE_2D, 0);
  cm_shader_unbind();
  glEnable(GL_DEPTH_TEST);
}

void cm_framebuffer_delete(CmFrameBuffer *framebuffer) {
  glDeleteFramebuffers(1, &framebuffer->fbo);
  glDeleteRenderbuffers(1, &framebuffer->rbo);
  glDeleteTextures(1, &framebuffer->texture);
}
