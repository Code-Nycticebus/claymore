#ifndef __CM_RENDERER2D_H__
#define __CM_RENDERER2D_H__

#include "renderer_defines.h"

void cm_renderer2d_begin(void);
void cm_renderer2d_end(void);

void cm_renderer2d_push_quad(const vec2 position, float z, const vec2 size);
void cm_renderer2d_push_quad_rotated(const vec2 position, float z,
                                     const vec2 size, float rotation);
void cm_renderer2d_push_quad_textured(const vec2 position, float z,
                                      const vec2 size, const vec2 texture_coord,
                                      const vec2 texture_size);
void cm_renderer2d_push_quad_textured_rotated(const vec2 position, float z,
                                              const vec2 size,
                                              const vec2 texture_coord,
                                              const vec2 texture_size,
                                              float rotation);
void cm_renderer2d_push_quad_color(const vec2 position, float z,
                                   const vec2 size, const vec4 color);
void cm_renderer2d_push_quad_color_rotated(const vec2 position, float z,
                                           const vec2 size, const vec4 color,
                                           float rotation);

#ifdef CM_RENDERER_PRIVATE
void cm_renderer2d_init(void);
void cm_renderer2d_shutdown(void);
#endif

#endif // !__CM_RENDERER2D_H__
