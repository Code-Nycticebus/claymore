
#ifndef __CLAYMORE_RENDERER2D_H__
#define __CLAYMORE_RENDERER2D_H__

#include "claymore/app/camera.h"
#include "claymore/defines.h" // IWYU pragma: export

#include "claymore/assets/font.h"
#include "claymore/assets/texture.h"

void cm_2D_begin(CmCamera2D *camera);
void cm_2D_end(void);

void cm_circle(const vec2 pos, const vec2 radius, const vec4 color);
void cm_quad(const vec2 pos, const vec2 size, float rotation, const vec4 color);

void cm_sprite(CmTexture2D *texture, const vec2 pos, const vec2 size,
               float rotation, const vec2 uv, const vec2 uv_size);

void cm_font_color(CmFont *font, vec4 color);
void cm_font(CmFont *font, const vec2 pos, Str text);

void cm_line(const vec2 from, const vec2 to);

/* ========= Renderer internal ========= */

typedef struct CmRenderer2D CmRenderer2D;

CmRenderer2D *cm_2D_internal_init(void);
void cm_2D_internal_free(void);
void cm_2D_internal_use(CmRenderer2D *r);

#endif /* !__CLAYMORE_RENDERER2D_H__ */
