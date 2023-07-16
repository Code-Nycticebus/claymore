#ifndef __CM_RENDERER2D_H__
#define __CM_RENDERER2D_H__

#include "cm.h"

#define CM_RENDERER2D_MAX_QUADS 12
#define CM_RENDERER2D_VERTECIES_PER_QUAD 4
#define CM_RENDERER2D_MAX_VERTECIES                                            \
  (CM_RENDERER2D_MAX_QUADS * CM_RENDERER2D_VERTECIES_PER_QUAD)
#define CM_RENDERER_INDICES_PER_SQUAD 6
#define CM_RENDERER2D_MAX_INDECIES                                             \
  (CM_RENDERER2D_MAX_QUADS * CM_RENDERER_INDICES_PER_SQUAD)

typedef struct {
  vec3 pos;
  vec4 color;
} Vertex2D;

typedef struct {
  GLuint vbo; // Vertex Buffer holds the data
  GLuint vao; // Vertex Array is there to be there???
  GLuint ibo; // Holds indecies to reduce repetition
  GLuint i_count;

  size_t vertecies_count;
  Vertex2D data[CM_RENDERER2D_MAX_VERTECIES];

  size_t indecies_count;
  uint32_t indecies[CM_RENDERER2D_MAX_INDECIES];
} RenderBufferData;

typedef struct {
  RenderBufferData buffer;

  struct {
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 mvp;
  } mvp;
} Renderer2D;

void cm_renderer_init_quad(Renderer2D *renderer, size_t size, int32_t x,
                           int32_t y, int32_t z);

#ifdef _CM_RENDERER_INTERNAL
void cm_renderer_init(Renderer2D *renderer);
void cm_renderer_draw(Renderer2D *renderer);
#endif // !_CM_RENDERER_INTERNAL

#endif // !__CM_RENDERER2D_H__
