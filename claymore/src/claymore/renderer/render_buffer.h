#ifndef __CM_RENDER_BUFFER_H__
#define __CM_RENDER_BUFFER_H__

#include "renderer_defines.h"
#include "shader.h"
CmVertexBuffer cm_vertex_buffer_create(const void *data, size_t bytes,
                                       CmBufferUsage buffer_usage);
void cm_vertex_buffer_update(CmVertexBuffer *vbo, const void *data,
                             size_t bytes);
void cm_vertex_buffer_delete(CmVertexBuffer *vbo);

CmVertexArray cm_vertex_array_create(void);
void cm_vertex_array_push_attrib(CmVertexArray *array, size_t count,
                                 size_t stride, size_t offset);
void cm_vertex_array_push_attrib_instanced(CmVertexArray *array, size_t count,
                                           size_t stride, size_t offset);

CmIndexBuffer cm_index_buffer_create(const uint32_t *indices, size_t count);
void cm_index_buffer_delete(CmIndexBuffer *ibo);

#endif // __CM_RENDER_BUFFER_H__
