#include "test.h"

#include "stb_image_write.h"
#include <stb_image.h>

typedef struct {
  CmFramebuffer fb;
  CmTexture frame;
} Test;

static void init(CmScene *scene) {
  Test *test = cm_scene_data(scene);

  RGFW_window *win = cm_app_window();
  test->fb = cm_framebuffer_create(&scene->gpu, (usize)win->r.w, (usize)win->r.h);
  test->frame = cm_framebuffer_attach_texture_color(&test->fb);
}

static void pre_update(CmScene *scene) {
  Test *test = cm_scene_data(scene);
  cm_framebuffer_begin(&test->fb);
}

static void post_update(CmScene *scene) {
  Test *test = cm_scene_data(scene);
  cm_framebuffer_end();
  cm_texture_bind(&test->frame, 0);

  Bytes data = cm_texture_read_bytes(&test->frame, &scene->arena);

  CmScene *child = cm_scene_child(scene, 0);
  Str name = cm_scene_name(child);
  Str filename = str_format(&scene->arena, "gen/" STR_FMT ".test", STR_ARG(name));

  if (file_exists(filename)) {
    Bytes test_data = file_read_bytes(filename, &scene->arena, ErrPanic);
    if (bytes_eq(test_data, data)) {
      file_write_bytes(filename, data, ErrPanic);
      cm_app_quit(TESTS_OK);
    } else {
      cm_app_quit(TESTS_ERR);
    }
  }

  arena_reset(&scene->arena);
}

static CmSceneInterface *interface(void) {
  static CmSceneInterface interface = {
      CM_SCENE(Test),
      .init = init,
      .pre_update = pre_update,
      .post_update = post_update,
  };
  return &interface;
}

CmScene *test_init(CmScene *parent, CmSceneInit test_scene) {
  CmScene *scene = cm_scene_push(parent, interface);
  cm_scene_push(scene, test_scene);

  return scene;
}
