#ifndef __CLAYMORE_RENDERER2D_H__
#define __CLAYMORE_RENDERER2D_H__

#include "claymore/app/camera.h"
#include "claymore/defines.h" // IWYU pragma: export

#include "claymore/assets/font.h"
#include "claymore/assets/texture.h"

// begins the 2D rendering process using the specified camera
void cm_2D_begin(CmCamera2D *camera);
// ends the 2D rendering process
void cm_2D_end(void);

// draws a circle at the specified position with the given radius and color
void cm_2D_circle(const vec2 pos, float radius, const vec4 color);

// draws a quad (rectangle) at the specified position, size, rotation, and color
void cm_2D_quad(const vec2 pos, const vec2 size, float rotation,
                const vec4 color);

// draws a sprite (textured quad) using the specified texture, position, size,
// rotation, and UV coordinates
void cm_2D_sprite(CmTexture2D *texture, const vec2 pos, const vec2 size,
                  float rotation, const vec2 uv, const vec2 uv_size);

// renders text at the specified position using the given font and text string
void cm_2D_text(CmFont *font, const vec2 pos, Str text);

// draws a line from the start position to the end position
void cm_2D_line(const vec2 from, const vec2 to);

/* ========= renderer internal ========= */

// forward declaration of the 2D renderer structure
typedef struct CmRenderer2D CmRenderer2D;

// initializes the internal 2D renderer and returns a pointer to it
CmRenderer2D *cm_2D_internal_init(void);
// frees the resources allocated for the internal 2D renderer
void cm_2D_internal_free(void);

// sets the renderer context to the specified 2D renderer instance
void cm_2D_internal_set_context(CmRenderer2D *r);

#endif /* !__CLAYMORE_RENDERER2D_H__ */
