#ifndef __CM_RENDER_BUFFER_H__
#define __CM_RENDER_BUFFER_H__

#include "renderer_defines.h"
#include "shader.h"

typedef struct {
  uint32_t id;
  size_t element_size;
  size_t element_count;
} CmVertexBuffer;

CmVertexBuffer cm_vertex_buffer_create(size_t count, size_t element_size,
                                       const void *data, GLenum draw_mode);
void cm_vertex_buffer_delete(CmVertexBuffer *vertex_buffer);

typedef struct {
  uint32_t id;
  size_t index;
  CmVertexBuffer *buffer;
} CmVertexAttribute;

CmVertexAttribute cm_vertex_attribute_create(CmVertexBuffer *vertex_buffer);
void cm_vertex_attribute_push(CmVertexAttribute *attr, size_t count,
                              GLenum type, size_t offset);
void cm_vertex_attribute_delete(CmVertexAttribute *vertex_attrib);

typedef struct {
  uint32_t id;
  size_t count;

  CmVertexAttribute *attrib;
} CmIndexBuffer;

CmIndexBuffer cm_index_buffer_create(CmVertexAttribute *attrib, size_t count,
                                     const uint32_t *indices, GLenum mode);
void cm_index_buffer_delete(CmIndexBuffer *index_buffer);

typedef struct {
  CmVertexBuffer vertex_buffer;
  CmVertexAttribute vertex_attribute;
  CmIndexBuffer index_buffer;
} CmRenderBuffer;

void cm_render_buffer_delete(CmRenderBuffer *render_buffer);

typedef struct {
  uint32_t fbo;
  uint32_t texture;
  uint32_t rbo;
  CmShader *shader;
  CmRenderBuffer *rb;
} CmFrameBuffer;

CmFrameBuffer cm_framebuffer_create(uint32_t width, uint32_t height);
void cm_framebuffer_bind(CmFrameBuffer *framebuffer);
void cm_framebuffer_unbind(void);
void cm_framebuffer_draw(CmFrameBuffer *framebuffer, CmRenderBuffer *rb,
                         CmShader *shader);
void cm_framebuffer_delete(CmFrameBuffer *framebuffer);

#endif // __CM_RENDER_BUFFER_H__
