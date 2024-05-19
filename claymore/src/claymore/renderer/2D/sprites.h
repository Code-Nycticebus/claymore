#ifndef __CLAYMORE_SPRITES_H__
#define __CLAYMORE_SPRITES_H__

#include "claymore/defines.h" // IWYU pragma: export

#include "claymore/app/camera.h"
#include "claymore/renderer/texture.h"

void cm_sprite(CmTexture2D *texture, const vec2 position, const vec2 size,
               float rotation, const vec2 uv, const vec2 uv_size);

void cm_sprite_internal_begin(CmCamera2D *camera);
void cm_sprite_internal_end(void);
usize cm_sprite_internal_init(void);
void cm_sprite_internal_free(void);

#endif /* __CLAYMORE_SPRITES_H__ */
