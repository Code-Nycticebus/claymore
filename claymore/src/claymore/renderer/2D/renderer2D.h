#ifndef __CLAYMORE_RENDERER2D_H__
#define __CLAYMORE_RENDERER2D_H__

#include "claymore/app/camera.h"
#include "claymore/defines.h" // IWYU pragma: export
#include "claymore/renderer/gpu.h"
#include "claymore/renderer/texture.h"

void cm_2D_begin(CmCamera2D *camera);
void cm_2D_end(void);

void cm_circle(const vec2 pos, const vec2 radius, const vec4 color);
void cm_quad(const vec2 pos, const vec2 size, float rotation, const vec4 color);

void cm_sprite(CmTexture2D *texture, const vec2 pos, const vec2 size,
               float rotation, const vec2 uv, const vec2 uv_size);

typedef struct CmFont CmFont;

CmFont *cm_font_from_bytes(CmGpu *gpu, Bytes bytes, float height);
CmFont *cm_font_from_file(CmGpu *gpu, Str path, float height, Error *error);
void cm_font(CmFont *font, const vec2 pos, Str text);

// INTERNAL

bool cm_2D_internal_init(void);
void cm_2D_internal_free(void);

void cm_circle_internal_begin(CmCamera2D *camera);
void cm_circle_internal_end(void);
usize cm_circle_internal_init(void);
void cm_circle_internal_free(void);

void cm_quad_internal_begin(CmCamera2D *camera);
void cm_quad_internal_end(void);
usize cm_quad_internal_init(void);
void cm_quad_internal_free(void);

void cm_sprite_internal_begin(CmCamera2D *camera);
void cm_sprite_internal_end(void);
usize cm_sprite_internal_init(void);
void cm_sprite_internal_free(void);

void cm_font_internal_begin(CmCamera2D *camera);
void cm_font_internal_end(void);
usize cm_font_internal_init(void);
void cm_font_internal_free(void);

#endif /* !__CLAYMORE_RENDERER2D_H__ */
