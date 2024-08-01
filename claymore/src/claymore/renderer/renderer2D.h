#ifndef __CLAYMORE_RENDERER2D_H__
#define __CLAYMORE_RENDERER2D_H__

#include "claymore/app/camera.h"
#include "claymore/defines.h" // IWYU pragma: export

#include "claymore/assets/font.h"
#include "claymore/assets/texture.h"

void cm_2D_begin(CmCamera2D *camera);
void cm_2D_end(void);

// circle
void cm_2D_circle(const vec2 pos, float radius, const vec4 color);

// quad
void cm_2D_quad(const vec2 pos, const vec2 size, const vec4 color);
void cm_2D_quad_rotated(const vec2 pos, const vec2 size, float r, const vec4 color);

// sprite
void cm_2D_sprite(CmTexture *texture, const vec2 pos, const vec2 size);
void cm_2D_sprite_rotated(CmTexture *texture, const vec2 pos, const vec2 size, float r);
void cm_2D_sprite_uv(CmTexture *texture, const vec2 pos, const vec2 size, const vec2 uv,
                     const vec2 uv_size);

// text
void cm_2D_text(CmFont *font, const vec2 pos, Str text);

// line
void cm_2D_line(const vec2 from, const vec2 to);

/* ========= renderer internal ========= */

typedef struct CmRenderer2D CmRenderer2D;

CmRenderer2D *cm_2D_internal_init(void);
void cm_2D_internal_free(void);
void cm_2D_internal_set_context(CmRenderer2D *r);

#endif /* !__CLAYMORE_RENDERER2D_H__ */
