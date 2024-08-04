#include "test.h"

#include "stb_image_write.h"
#include <stb_image.h>

typedef struct {
  bool write;
  CmFramebuffer fb;
  CmTexture frame;
  Error *error;
} Writer;

static void pre_update(CmScene *scene) {
  Writer *test = cm_scene_data(scene);
  cm_framebuffer_begin(&test->fb);
}

static void post_update(CmScene *scene) {
  Writer *test = cm_scene_data(scene);
  cm_framebuffer_end(&test->fb);
  cm_texture_bind(&test->frame, 0);

  Bytes data = cm_texture_read_bytes(&test->frame, &scene->arena);

  CmScene *child = cm_scene_child(scene, 0);
  Str name = cm_scene_name(child);
  cebus_assert(0 < name.len, "name needs to be longer");
  Str filename = str_format(&scene->arena, "gen/" STR_FMT ".test", STR_ARG(name));

  if (!file_exists(filename)) {
    error_emit(test->error, TESTS_ERROR, "Test file does not exist: '" STR_FMT "'", STR_ARG(name));
    goto defer;
  }

  Bytes test_data = file_read_bytes(filename, &scene->arena, ErrPanic);
  u64 expected_data_hash = u64_from_le_bytes(test_data);
  if (expected_data_hash == bytes_hash(data)) {
    cm_scene_delete(scene);
  } else {
    error_emit(test->error, TESTS_FAILED, "Test '" STR_FMT "' failed", STR_ARG(name));
    goto defer;
  }

defer:
  arena_reset(&scene->arena);
}

static CmSceneInterface *interface(void) {
  static CmSceneInterface interface = {
      CM_SCENE(Writer),
      .pre_update = pre_update,
      .post_update = post_update,
  };
  return &interface;
}

CmScene *test_init(CmScene *parent, u32 width, u32 height, Error *error, CmSceneInit test_scene) {
  CmScene *scene = cm_scene_push(parent, interface);
  cm_scene_push(scene, test_scene);
  Writer *test = cm_scene_data(scene);
  test->error = error;

  Str name = cm_scene_name(cm_scene_child(scene, 0));
  cebus_log_info("TESTING: " STR_FMT, STR_ARG(name));

  test->fb = cm_framebuffer_create(&scene->gpu, width, height);
  test->frame = cm_framebuffer_attach_texture_color(&test->fb);

  return scene;
}
