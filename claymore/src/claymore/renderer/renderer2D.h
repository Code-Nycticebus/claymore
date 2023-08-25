#ifndef __CM_RENDERER2D_H__
#define __CM_RENDERER2D_H__

#include "renderer_defines.h"

#define CM_RENDERER2D_MAX_QUADS 3000
#define CM_RENDERER2D_VERTICES_PER_QUAD 4
#define CM_RENDERER2D_MAX_VERTICES                                             \
  (CM_RENDERER2D_MAX_QUADS * CM_RENDERER2D_VERTICES_PER_QUAD)
#define CM_RENDERER_INDICES_PER_SQUAD 6
#define CM_RENDERER2D_MAX_INDECIES                                             \
  (CM_RENDERER2D_MAX_QUADS * CM_RENDERER_INDICES_PER_SQUAD)

void cm_renderer2d_begin(void);
void cm_renderer2d_end(void);

void cm_renderer2d_push_quad(const vec2 position, float z, const vec2 size,
                             vec2 texture_coord, vec2 texture_size);

void cm_renderer2d_push_quad_rotated(const vec2 position, float z,
                                     const vec2 size, vec2 texture_coord,
                                     vec2 texture_size, float rotation);

void cm_renderer2d_push_quad_color(const vec2 position, float z,
                                   const vec2 size, const vec4 color);
void cm_renderer2d_push_quad_color_rotated(const vec2 position, float z,
                                           const vec2 size, const vec4 color,
                                           float rotation);

void cm_renderer2d_init(void);
void cm_renderer2d_shutdown(void);
void cm_renderer2d_flush(void);

#endif // !__CM_RENDERER2D_H__
