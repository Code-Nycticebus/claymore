#ifndef __CLAYMORE_RENDERER2D_H__
#define __CLAYMORE_RENDERER2D_H__

#include "claymore/app/camera.h"
#include "claymore/defines.h" // IWYU pragma: export

#include "font.h"    // IWYU pragma: export
#include "quads.h"   // IWYU pragma: export
#include "sprites.h" // IWYU pragma: export

void cm_renderer2d_begin(CmCamera2D *camera);
void cm_renderer2d_end(void);

bool cm_renderer2d_internal_init(void);
void cm_renderer2d_internal_free(void);

#endif /* !__CLAYMORE_RENDERER2D_H__ */
