#ifndef __CM_RENDER_COMMAND_H__
#define __CM_RENDER_COMMAND_H__

#include "render_buffer.h"
#include "renderer_defines.h"

void cm_renderer_draw_indexed(CmRenderBuffer *render_buffer,
                              size_t indices_count);

void cm_renderer_clear(void);
void cm_renderer_set_clear_color(vec4s color);

#endif /* !__CM_RENDER_COMMAND_H__ */
