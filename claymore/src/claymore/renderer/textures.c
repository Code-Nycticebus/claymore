#include "textures.h"
#include <stb_image.h>

CmTexture cm_texture_create(const char *filename) {
  int32_t height;
  int32_t width;
  int32_t bpp;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *texture_buffer = stbi_load(filename, &width, &height, &bpp, 4);
  const char *fail = stbi_failure_reason();
  if (!texture_buffer || fail) {
    cm_log_error("Failed to load texture: %s\n", filename);
    cm_log_error("%s\n", fail);
    return (CmTexture){0};
  }

  CmTexture texture =
      cm_texture_create_from_memory(width, height, texture_buffer, bpp);

  stbi_image_free(texture_buffer);

  return texture;
}

CmTexture cm_texture_create_from_memory(uint32_t width, uint32_t height,
                                        const void *data, int32_t bpp) {
  CmTexture texture = {
      .id = 0,
      .height = height,
      .width = width,
      .bpp = bpp,
      .internal = GL_RGBA8,
      .format = GL_RGBA,
      .type = GL_UNSIGNED_BYTE,
  };

  glGenTextures(1, &texture.id);
  glBindTexture(GL_TEXTURE_2D, texture.id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, texture.internal, texture.width,
               texture.height, 0, texture.format, texture.type, data);

  glBindTexture(GL_TEXTURE_2D, 0);

  return texture;
}

void cm_texture_update(CmTexture *texture, const void *data) {
  glBindTexture(GL_TEXTURE_2D, texture->id);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture->width, texture->height,
                  texture->format, texture->type, data);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void cm_texture_delete(CmTexture *texture) {
  glDeleteTextures(1, &texture->id);
}

void cm_texture_bind(CmTexture *texture, uint32_t slot) {
  glBindTextureUnit(slot, texture->id);
}

void cm_texture_unbind(uint32_t slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, 0);
}
