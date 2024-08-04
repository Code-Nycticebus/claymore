#include "tests.h"

typedef struct {
  CmCamera2D camera;
  CmTexture sprite;
} Renderer2D;

static void init(CmScene *scene) {
  Renderer2D *renderer = cm_scene_data(scene);
  const float size = 100.f;
  cm_camera2D_ortho(&renderer->camera, (vec2){0}, (f32)HEIGHT / (f32)WIDHT, size);

  static u8 data[] = {
      0,   255, 255, //
      255, 0,   255, //
      255, 0,   255, //
      0,   255, 255, //
  };
  renderer->sprite = cm_texture_from_bytes(&scene->gpu, (u8 *)data,
                                           (CmTextureFormat){
                                               .w = 2,
                                               .h = 2,
                                               .bpp = 3,
                                               .mag = GL_NEAREST,
                                           });
}

static void frame_update(CmScene *scene) {
  Renderer2D *renderer = cm_scene_data(scene);

  cm_2D_begin(&renderer->camera);

  cm_2D_quad((vec2){40, -30}, (vec2){40, 40}, (vec4){0.7f, 0.2f, 0.2f, 1});
  cm_2D_quad_rotated((vec2){-30, 25}, (vec2){50, 50}, glm_rad(30), (vec4){0.25, 0.25, 0.8f, 1});
  cm_2D_circle((vec2){-20, -50}, 30, (vec4){0.3f, 0.7f, 0.2f, 1});

  cm_2D_sprite(&renderer->sprite, (vec2){31, 45}, (vec2){35, 35});

  cm_2D_line((vec2){20, 30}, (vec2){-30, 50});
  cm_2D_line((vec2){-30, 50}, (vec2){-80, -60});

  cm_2D_end();
}

CmSceneInterface *renderer2D(void) {
  static CmSceneInterface interface = {
      CM_SCENE(Renderer2D),
      .init = init,
      .frame_update = frame_update,
  };
  return &interface;
}
