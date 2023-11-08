#include "textures.h"
#include <stb_image.h>

CmTexture cm_texture_create(const char *filename) {
  CmTexture texture = {0};
  stbi_set_flip_vertically_on_load(true);
  unsigned char *texture_buffer =
      stbi_load(filename, &texture.width, &texture.height, &texture.bpp, 4);
  const char *fail = stbi_failure_reason();
  if (!texture_buffer || fail) {
    cm_log_error("Failed to load texture: %s\n", filename);
    cm_log_error("%s\n", fail);
    return (CmTexture){0};
  }

  glGenTextures(1, &texture.id);
  glBindTexture(GL_TEXTURE_2D, texture.id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.width, texture.height, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, texture_buffer);

  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(texture_buffer);

  return texture;
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
