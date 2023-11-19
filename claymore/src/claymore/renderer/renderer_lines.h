#ifndef __CM_RENDERER_LINES_H__
#define __CM_RENDERER_LINES_H__

#include "cm.h"

void cm_renderer_lines_init(void);
void cm_renderer_lines_shutdown(void);

void cm_renderer_lines_begin(void);
void cm_renderer_lines_end(void);

void cm_renderer_lines_push(vec3s pos, vec4s color);

#endif // !__CM_RENDERER_LINES_H__
