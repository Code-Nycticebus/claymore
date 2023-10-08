#ifndef __CM_RENDERER3D_H__
#define __CM_RENDERER3D_H__

#include "renderer_defines.h"

void cm_renderer3d_begin(void);
void cm_renderer3d_end(void);

void cm_renderer3d_push_cube(vec3s position, vec3s size);

#ifdef CM_RENDERER_PRIVATE
void cm_renderer3d_init(void);
void cm_renderer3d_shutdown(void);
#endif

#endif // !__CM_RENDERER3D_H__
