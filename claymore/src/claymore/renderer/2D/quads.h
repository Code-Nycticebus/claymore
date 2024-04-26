#ifndef __CLAYMORE_QUADS_H__
#define __CLAYMORE_QUADS_H__

#include "claymore/defines.h" // IWYU pragma: export

void cm_quad_begin(mat4 mvp /* CmCamera* camera */);
void cm_quad_end(void);

void cm_quad_push(const vec2 position, const vec2 size, float rotation,
                  const vec4 color);

usize cm_quad_internal_init(void);
void cm_quad_internal_free(void);

#endif /* __CLAYMORE_QUADS_H__ */
