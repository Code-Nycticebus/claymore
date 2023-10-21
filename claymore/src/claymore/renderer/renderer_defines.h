#ifndef __CM_RENDERER_DEFINES_H__
#define __CM_RENDERER_DEFINES_H__

#include "cm.h"

typedef enum {
  CM_BUFFER_STATIC = GL_STATIC_DRAW,
  CM_BUFFER_DYNAMIC = GL_DYNAMIC_DRAW,
  CM_BUFFER_INSTANCED = GL_DYNAMIC_DRAW,
} CmBufferUsage;

typedef struct {
  GLuint id;
  size_t element_size;
  size_t element_count;
} CmVertexBuffer;

typedef struct {
  GLuint id;
  size_t indices_count;
} CmIndexBuffer;

typedef struct {
  GLuint id;
  size_t index;
} CmVertexArray;

#define CM_RENDER_BUFFER_MAX 5

typedef struct {
  size_t instance_count;
  size_t buffer_count;
  CmVertexBuffer buffers[CM_RENDER_BUFFER_MAX];

  CmVertexArray vertex_array;
  CmIndexBuffer indices;
} CmRenderBuffer;

#endif /* !__CM_RENDERER_DEFINES_H__ */
