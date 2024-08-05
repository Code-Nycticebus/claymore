#include "record.h"

#include "stb_image_write.h"
#include <stb_image.h>

typedef struct {
  CmFramebuffer fb;
  CmTexture frame;
  CmCamera2D camera;
} Player;

static void pre_update(CmScene *scene) {
  Player *test = cm_scene_data(scene);
  cm_framebuffer_begin(&test->fb);

  cm_camera2D_ortho(&test->camera, (vec2){0}, cm_app_aspect(), 1);
}

static void post_update(CmScene *scene) {
  Player *test = cm_scene_data(scene);
  cm_framebuffer_end(&test->fb);
  cm_texture_bind(&test->frame, 0);

  cm_2D_begin(&test->camera);
  cm_2D_sprite(&test->frame, (vec2){-1.f, 1.f}, (vec2){2.f, -2.f});
  cm_2D_end();
}

static CmSceneInterface *interface(void) {
  static CmSceneInterface interface = {
      CM_SCENE(Player),
      .pre_update = pre_update,
      .post_update = post_update,
  };
  return &interface;
}

CmScene *player_push(CmScene *parent, u32 width, u32 height, CmSceneInit test_scene) {
  CmScene *scene = cm_scene_push(parent, interface);
  cm_scene_push(scene, test_scene);

  Player *writer = cm_scene_data(scene);

  writer->fb = cm_framebuffer_create(&scene->gpu, width, height);
  writer->frame = cm_framebuffer_attach_texture_color(&writer->fb);

  return scene;
}
