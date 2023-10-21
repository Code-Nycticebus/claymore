#ifndef __CM_RENDER_BUFFER_H__
#define __CM_RENDER_BUFFER_H__

#include "renderer_defines.h"
#include "shader.h"
CmVertexBuffer cm_vertex_buffer_create(const void *data, size_t bytes,
                                       CmBufferUsage buffer_usage);

CmVertexArray cm_vertex_array_create(void);
void cm_vertex_array_push_attrib(CmVertexArray *array, size_t count,
                                 size_t stride, size_t offset);
void cm_vertex_array_push_attrib_instanced(CmVertexArray *array, size_t count,
                                           size_t stride, size_t offset);
CmIndexBuffer cm_index_buffer_create(const uint32_t *indices, size_t count);

CmRenderBuffer cm_render_buffer_create(const vec3s *positions, size_t count,
                                       const uint32_t *indices,
                                       size_t indices_count);

CmVertexBuffer *cm_buffer_attach_vec2(CmRenderBuffer *buffer, const vec2s *data,
                                      size_t count, CmBufferUsage buffer_usage);
void cm_buffer_update_vec2(CmRenderBuffer *buffer, CmVertexBuffer *vb,
                           const vec2s *data, size_t count);

CmVertexBuffer *cm_buffer_attach_vec3(CmRenderBuffer *buffer, const vec3s *data,
                                      size_t count, CmBufferUsage buffer_usage);
void cm_buffer_update_vec3(CmRenderBuffer *buffer, CmVertexBuffer *vb,
                           const vec3s *data, size_t count);

CmVertexBuffer *cm_buffer_attach_vec4(CmRenderBuffer *buffer, const vec2s *data,
                                      size_t count, CmBufferUsage buffer_usage);
void cm_buffer_update_vec4(CmRenderBuffer *buffer, CmVertexBuffer *vb,
                           const vec4s *data, size_t count);

CmVertexBuffer *cm_buffer_attach_mat4(CmRenderBuffer *buffer, const mat4s *data,
                                      size_t count);
void cm_buffer_update_mat4(CmRenderBuffer *buffer, CmVertexBuffer *vb,
                           const mat4s *data, size_t count);

void cm_buffer_draw(CmRenderBuffer *buffer);

typedef struct {
  uint32_t fbo;
  uint32_t texture;
  uint32_t rbo;
  CmRenderBuffer *rb;
} CmFrameBuffer;

CmFrameBuffer cm_framebuffer_create(uint32_t width, uint32_t height);
void cm_framebuffer_bind(CmFrameBuffer *framebuffer);
void cm_framebuffer_unbind(void);
void cm_framebuffer_draw(CmFrameBuffer *framebuffer, CmRenderBuffer *rb);
void cm_framebuffer_delete(CmFrameBuffer *framebuffer);

#endif // __CM_RENDER_BUFFER_H__
