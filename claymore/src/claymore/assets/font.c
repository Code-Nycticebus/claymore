#include "font.h"

#include <glad.h>

CmFont *cm_font_from_bytes(CmGpu *gpu, Bytes bytes, float height) {
  CmFont *font = arena_calloc(gpu->arena, sizeof(CmFont));

  GLint max_texture_size;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);

  const size_t ttf_bitmap_resolution = glm_min(512 * ceilf(height / 100.F), max_texture_size);
  const size_t ttf_bitmap_size = ttf_bitmap_resolution * ttf_bitmap_resolution;

  Arena temp = {0};

  uint8_t *ttf_bitmap = arena_calloc(&temp, sizeof(uint8_t) * ttf_bitmap_size);
  stbtt_BakeFontBitmap(bytes.data, 0, height, ttf_bitmap, ttf_bitmap_resolution,
                       ttf_bitmap_resolution, FONT_CHAR_MIN, FONT_CHAR_MAX, font->cdata);

  font->texture_id = cm_gpu_texture(gpu);
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, font->texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ttf_bitmap_resolution, ttf_bitmap_resolution, 0, GL_RED,
               GL_UNSIGNED_BYTE, ttf_bitmap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  arena_free(&temp);

  font->height = height;
  font->ttf_resoulution = ttf_bitmap_resolution;
  glm_vec4_one(font->color);

  return font;
}

CmFont *cm_font_from_file(CmGpu *gpu, Str path, float height, Error *error) {
  CmFont *font = NULL;
  Arena temp = {0};

  Bytes ttf_buffer = file_read_bytes(path, &temp, error);
  error_propagate(error, { goto defer; });

  font = cm_font_from_bytes(gpu, ttf_buffer, height);

defer:
  arena_free(&temp);
  return font;
}

void cm_font_color(CmFont *font, vec4 color) { glm_vec4_copy(color, font->color); }
