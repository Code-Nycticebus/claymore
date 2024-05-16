#ifndef __CLAYMORE_QUADS_H__
#define __CLAYMORE_QUADS_H__

#include "claymore/app/camera.h"
#include "claymore/defines.h" // IWYU pragma: export

void cm_quad_push(const vec2 position, const vec2 size, float rotation,
                  const vec4 color);

void cm_quad_internal_begin(CmCamera2D *camera);
void cm_quad_internal_end(void);
usize cm_quad_internal_init(void);
void cm_quad_internal_free(void);

#endif /* __CLAYMORE_QUADS_H__ */
