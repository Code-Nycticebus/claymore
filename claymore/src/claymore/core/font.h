#ifndef __CM_FONT_H__
#define __CM_FONT_H__

#include "cm.h"

#include "claymore/core/camera.h"

void cm_font_init(const char *filename, const float font_height);

void cm_font_draw(mat4 mvp, float x, float y, size_t len, const char *text);

void cm_font_free(void);

#endif /* !__CM_FONT_H__ */
