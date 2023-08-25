#ifndef __CM_FONT_H__
#define __CM_FONT_H__

#include "cm.h"

typedef struct CmFont CmFont;

CmFont *cm_font_init(const char *filename, const float font_height);

void cm_font_draw(CmFont *font, mat4 mvp, float x, float y, float z, size_t len,
                  const char *text);

void cm_font_free(CmFont *font);

#endif /* !__CM_FONT_H__ */
