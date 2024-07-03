#ifndef __CLAYMORE_FONT_H__
#define __CLAYMORE_FONT_H__

#include "claymore/defines.h" // IWYU pragma: export
#include "claymore/renderer/gpu.h"

#include <stb_truetype.h>

#define FONT_CHAR_MIN 32
#define FONT_CHAR_MAX 96

typedef struct {
  u32 texture_id;
  stbtt_bakedchar cdata[FONT_CHAR_MAX];
  float height;
  size_t ttf_resoulution;
  vec4 color;
} CmFont;

CmFont *cm_font_from_bytes(CmGpu *gpu, Bytes bytes, float height);
CmFont *cm_font_from_file(CmGpu *gpu, Str path, float height, Error *error);

void cm_font_color(CmFont *font, vec4 color);

#endif /* !__CLAYMORE_FONT_H__ */
