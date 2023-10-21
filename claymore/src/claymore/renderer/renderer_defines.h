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
  size_t size;
  CmBufferUsage usage;
} CmVertexBuffer;

typedef struct {
  GLuint id;
  size_t count;
} CmIndexBuffer;

typedef struct {
  GLuint id;
  size_t index;
} CmVertexArray;

#endif /* !__CM_RENDERER_DEFINES_H__ */
