#ifndef __CM_RENDERER2D_H__
#define __CM_RENDERER2D_H__

#include "renderer_defines.h"

#define CM_RENDERER2D_MAX_QUADS 2000
#define CM_RENDERER2D_VERTECIES_PER_QUAD 4
#define CM_RENDERER2D_MAX_VERTECIES                                            \
  (CM_RENDERER2D_MAX_QUADS * CM_RENDERER2D_VERTECIES_PER_QUAD)
#define CM_RENDERER_INDICES_PER_SQUAD 6
#define CM_RENDERER2D_MAX_INDECIES                                             \
  (CM_RENDERER2D_MAX_QUADS * CM_RENDERER_INDICES_PER_SQUAD)

void cm_renderer2d_begin(void);
void cm_renderer2d_end(void);

void cm_renderer2d_push_quad(const vec2 position, float z, const vec2 size);
void cm_renderer2d_push_quad_color(const vec2 position, float z,
                                   const vec2 size, const vec4 color);

#ifdef _CM_RENDERER_INTERNAL
void cm_renderer_init(void);
void cm_renderer_shutdown(void);
void cm_renderer2d_flush(void);
#endif // !_CM_RENDERER_INTERNAL

#endif // !__CM_RENDERER2D_H__
