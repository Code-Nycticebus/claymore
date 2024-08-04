#include "writer.h"

#include "stb_image_write.h"
#include <stb_image.h>

typedef struct {
  CmFramebuffer fb;
  CmTexture frame;
} Writer;

static void pre_update(CmScene *scene) {
  Writer *test = cm_scene_data(scene);
  cm_framebuffer_begin(&test->fb);
}

static void post_update(CmScene *scene) {
  Writer *test = cm_scene_data(scene);
  cm_framebuffer_end();
  cm_texture_bind(&test->frame, 0);

  Bytes data = cm_texture_read_bytes(&test->frame, &scene->arena);

  CmScene *child = cm_scene_child(scene, 0);
  Str name = cm_scene_name(child);

  Str filename = str_format(&scene->arena, "gen/" STR_FMT ".test", STR_ARG(name));
  file_write_bytes(filename, u64_to_le_bytes(bytes_hash(data), &scene->arena), ErrPanic);

  Str outfile = str_format(&scene->arena, "gen/" STR_FMT ".png", STR_ARG(name));
  stbi_flip_vertically_on_write(true);
  stbi_write_png(outfile.data, (i32)test->frame.width, (i32)test->frame.height,
                 (i32)test->frame.bpp, data.data, (i32)(test->frame.width * test->frame.bpp));

  arena_reset(&scene->arena);
  cm_scene_delete(scene);
}

static CmSceneInterface *interface(void) {
  static CmSceneInterface interface = {
      CM_SCENE(Writer),
      .pre_update = pre_update,
      .post_update = post_update,
  };
  return &interface;
}

CmScene *writer_push(CmScene *parent, u32 width, u32 height, CmSceneInit test_scene) {
  CmScene *scene = cm_scene_push(parent, interface);
  cm_scene_push(scene, test_scene);
  Writer *writer = cm_scene_data(scene);

  writer->fb = cm_framebuffer_create(&scene->gpu, width, height);
  writer->frame = cm_framebuffer_attach_texture_color(&writer->fb);

  return scene;
}
