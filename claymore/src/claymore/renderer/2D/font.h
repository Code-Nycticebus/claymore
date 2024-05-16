#ifndef __CLAYMORE_FONT_H__
#define __CLAYMORE_FONT_H__

#include "claymore/defines.h" // IWYU pragma: export
#include "claymore/renderer/gpu.h"

#include "claymore/app/camera.h"

typedef struct CmFont CmFont;

CmFont *cm_font_init(CmGpu *gpu, Str filename, float font_height, Error *error);
void cm_font_draw(CmFont *font, const vec2 pos, Str text);

void cm_font_begin(CmCamera2D *camera);
void cm_font_end(void);

usize cm_font_internal_init(void);
void cm_font_internal_free(void);

#endif /* ifndef __CLAYMORE_FONT_H__ */
