#ifndef __CM_FONT_H__
#define __CM_FONT_H__

#include "cm.h"

typedef struct CmFont CmFont;

CmFont *cm_font_init(const char *filename, float font_height);

void cm_font_draw(CmFont *font, mat4s mvp, float x, float y, float z,
                  size_t len, const char *text);

void cm_font_draw_cstr(CmFont *font, mat4s mvp, float x, float y, float z,
                       const char *text);

void cm_font_free(CmFont *font);

#endif /* !__CM_FONT_H__ */
