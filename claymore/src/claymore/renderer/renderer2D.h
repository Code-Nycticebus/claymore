#ifndef __CM_RENDERER2D_H__
#define __CM_RENDERER2D_H__

#include "renderer_defines.h"

void cm_renderer2d_begin(void);
void cm_renderer2d_end(void);

void cm_renderer2d_push_quad(vec2s position, float z, vec2s size);
void cm_renderer2d_push_quad_rotated(vec2s position, float z, vec2s size,
                                     float rad);
void cm_renderer2d_push_quad_textured(vec2s position, float z, vec2s size,
                                      vec2s uv_pos, vec2s uv_size);
void cm_renderer2d_push_quad_textured_rotated(vec2s position, float z,
                                              vec2s size, vec2s uv_pos,
                                              vec2s uv_size, float rad);
void cm_renderer2d_push_quad_color(vec2s position, float z, vec2s size,
                                   vec4s color);
void cm_renderer2d_push_quad_color_rotated(vec2s position, float z, vec2s size,
                                           vec4s color, float rad);

#ifdef CM_RENDERER_PRIVATE
void cm_renderer2d_init(void);
void cm_renderer2d_shutdown(void);
#endif

#endif // !__CM_RENDERER2D_H__
