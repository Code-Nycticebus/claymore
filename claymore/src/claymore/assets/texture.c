#include "texture.h"

#include <glad.h>
#include <stb_image.h>

typedef struct {
  GLint internal;
  GLenum format;
  GLenum type;
} TextureInternalFormat;

static const TextureInternalFormat texture_format[] = {
    [1] = /* bpp */
    {
        .internal = GL_R8,
        .format = GL_RED,
        .type = GL_UNSIGNED_BYTE,
    },
    [2] = /* bpp */
    {
        .internal = GL_RG8,
        .format = GL_RG,
        .type = GL_UNSIGNED_BYTE,
    },
    [3] = /* bpp */
    {
        .internal = GL_RGB8,
        .format = GL_RGB,
        .type = GL_UNSIGNED_BYTE,
    },
    [4] = /* bpp */
    {
        .internal = GL_RGBA8,
        .format = GL_RGBA,
        .type = GL_UNSIGNED_BYTE,
    },

};

CmTexture cm_texture_from_file(CmGpu *gpu, Str filename, Error *error) {
  CmTexture texture = {0};
  Arena temp = {0};

  Str path = str_copy(filename, &temp);
  int32_t height;
  int32_t width;
  int32_t bpp;

  // stbi_set_flip_vertically_on_load(false);
  u8 *texture_buffer = stbi_load(path.data, &width, &height, &bpp, 4);
  const char *fail = stbi_failure_reason();
  if (!texture_buffer || fail) {
    error_emit(error, -1, "Failed to load texture: " STR_FMT ": %s", STR_ARG(filename), fail);
    goto defer;
  }
  texture = cm_texture_from_bytes(gpu, texture_buffer,
                                  (CmTextureFormat){
                                      .w = width,
                                      .h = height,
                                      .bpp = bpp,
                                  });

defer:
  stbi_image_free(texture_buffer);
  arena_free(&temp);
  return texture;
}

CmTexture cm_texture_from_bytes(CmGpu *gpu, const u8 *data, CmTextureFormat format) {
  cebus_assert(0 < format.bpp && format.bpp <= 4, "bytes per pixel needs to be 1-4");
  CmTexture texture = {
      .id = 0,
      .width = format.w,
      .height = format.w,
      .bpp = format.bpp,
  };

  texture.id = cm_gpu_texture(gpu);
  glBindTexture(GL_TEXTURE_2D, texture.id);

  const usize width = format.w * format.bpp;
  if (width % 4) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if (format.mag) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, format.mag);
  }

  /* keep default GL_REPEAT */
  if (format.wrap) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format.wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format.wrap);
  }

  const TextureInternalFormat f = texture_format[format.bpp];
  glTexImage2D(GL_TEXTURE_2D, 0, f.internal, format.w, format.h, 0, f.format, f.type, data);

  glBindTexture(GL_TEXTURE_2D, 0);
  return texture;
}

Bytes cm_texture_read_bytes(CmTexture *texture, Arena *arena) {
  const usize size = texture->width * texture->height * texture->bpp;
  void *data = arena_alloc(arena, size);
  const TextureInternalFormat format = texture_format[texture->bpp];
  glGetTexImage(GL_TEXTURE_2D, 0, format.format, format.type, data);
  return bytes_from_parts(size, data);
}

void cm_texture_bind(CmTexture *texture, uint32_t slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, texture->id);
}

void cm_texture_unbind(uint32_t slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, 0);
}
