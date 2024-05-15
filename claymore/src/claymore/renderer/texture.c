#include "texture.h"

#include <GL/glew.h>
#include <stb_image.h>

static const struct {
  GLint internal;
  GLenum format;
  GLenum type;
} TextureFormat[] = {
    [CM_TEXTURE_R] =
        {
            .internal = GL_R8,
            .format = GL_RED,
            .type = GL_UNSIGNED_BYTE,
        },
    [CM_TEXTURE_RGBA] =
        {
            .internal = GL_RGBA8,
            .format = GL_RGBA,
            .type = GL_UNSIGNED_BYTE,
        },

};

CmTexture2D cm_texture_from_file(CmGpu *gpu, Str filename, Error *error) {
  CmTexture2D texture = {0};
  Arena temp = {0};

  Str path = str_copy(filename, &temp);
  int32_t height;
  int32_t width;
  int32_t bpp;

  // stbi_set_flip_vertically_on_load(false);
  unsigned char *texture_buffer =
      stbi_load(path.data, &width, &height, &bpp, 4);
  const char *fail = stbi_failure_reason();
  if (!texture_buffer || fail) {
    error_emit(error, -1, "Failed to load texture: " STR_FMT ": %s",
               STR_ARG(filename), fail);
    goto defer;
  }
  cebus_assert(bpp == 4, "Pixel format not supported!");

  texture = cm_texture_from_memory(gpu, width, height, texture_buffer,
                                   CM_TEXTURE_RGBA);

  stbi_image_free(texture_buffer);

defer:
  arena_free(&temp);
  return texture;
}

CmTexture2D cm_texture_from_memory(CmGpu *gpu, usize width, usize height,
                                   const void *data, CmTextureFormat format) {
  CmTexture2D texture = {
      .id = 0,
      .width = width,
      .height = height,
  };

  texture.id = cm_gpu_texture(gpu);
  glBindTexture(GL_TEXTURE_2D, texture.id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, TextureFormat[format].internal, texture.width,
               texture.height, 0, TextureFormat[format].format,
               TextureFormat[format].type, data);

  glBindTexture(GL_TEXTURE_2D, 0);
  return texture;
}

void cm_texture_bind(CmTexture2D *texture, uint32_t slot) {
  glBindTextureUnit(slot, texture->id);
}

void cm_texture_unbind(uint32_t slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, 0);
}
