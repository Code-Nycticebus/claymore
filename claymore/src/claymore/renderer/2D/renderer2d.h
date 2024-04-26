#ifndef __CLAYMORE_RENDERER2D_H__
#define __CLAYMORE_RENDERER2D_H__

#include "claymore/defines.h" // IWYU pragma: export

#include "quads.h"   // IWYU pragma: export
#include "sprites.h" // IWYU pragma: export

bool cm_renderer2d_internal_init(void);
void cm_renderer2d_internal_free(void);

#endif /* !__CLAYMORE_RENDERER2D_H__ */
