#ifndef __CLAYMORE_CIRCLES_H__
#define __CLAYMORE_CIRCLES_H__

#include "claymore/defines.h" // IWYU pragma: export

#include "claymore/app/camera.h"

void cm_circle(const vec2 position, float radius, const vec4 color);

void cm_circle_internal_begin(CmCamera2D *camera);
void cm_circle_internal_end(void);
usize cm_circle_internal_init(void);
void cm_circle_internal_free(void);

#endif /* !__CLAYMORE_CIRCLES_H__ */
