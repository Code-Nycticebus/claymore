#ifndef __CLAYMORE_SPRITES_H__
#define __CLAYMORE_SPRITES_H__

#include "claymore/app/camera.h"
#include "claymore/defines.h" // IWYU pragma: export
#include "claymore/renderer/texture.h"

void cm_sprite_begin(CmCamera2D *camera);
void cm_sprite_end(void);

void cm_sprite_push(CmTexture2D *texture, const vec2 position, const vec2 size,
                    float rotation, const vec2 uv, const vec2 uv_size);

usize cm_sprite_internal_init(void);
void cm_sprite_internal_free(void);

#endif /* __CLAYMORE_SPRITES_H__ */
