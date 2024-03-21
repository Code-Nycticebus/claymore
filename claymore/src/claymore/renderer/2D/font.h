#ifndef __CLAYMORE_FONT_H__
#define __CLAYMORE_FONT_H__

#include "claymore/defines.h" // IWYU pragma: export
#include "claymore/renderer/gpu.h"

typedef struct CmFont CmFont;

CmFont *cm_font_init(CmGpu *gpu, Str filename, float font_height, Error *error);
void cm_font_draw(CmFont *font, mat4 mvp, const vec2 pos, Str text);

#endif /* ifndef __CLAYMORE_FONT_H__ */
