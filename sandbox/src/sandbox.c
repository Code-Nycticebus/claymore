#include "claymore/entrypoint.h"

CmSceneInterface *fps_scene_init(void);

typedef struct {
  float zoom;
  vec3 position;
  mat4 view;
  mat4 projection;
} Camera;

typedef struct {
  Camera camera;
  CmTexture2D texture;
} Sandbox;

static void event(CmScene *scene, CmEvent *event) {
  Sandbox *sandbox = scene->data;
  (void)sandbox, (void)event;
  cm_event_key(event, {
    if (key->action == CM_KEY_PRESS && key->code == CM_KEY_ESCAPE) {
      cm_window_close(true);
    }
  });
}

static void init(CmScene *scene) {
  Sandbox *sandbox = arena_alloc(&scene->arena, sizeof(Sandbox));
  scene->data = sandbox;
  sandbox->texture = cm_texture_from_file(
      STR("sandbox/res/textures/claymore-sword.png"), ErrPanic);
  cm_scene_push(scene, fps_scene_init);
}

static void update(CmScene *scene, double dt) {
  (void)dt;
  Sandbox *sandbox = scene->data;
  mat4 vp = GLM_MAT4_IDENTITY_INIT;
  cm_sprite_begin(vp, &sandbox->texture);
  cm_sprite_push((vec2){-.5f, -.5f}, (vec2){1.f, 1.f}, 0, (vec2){0, 0},
                 (vec2){1, 1});
  cm_sprite_end();
}

static CmSceneInterface *scene_init(void) {
  static CmSceneInterface sandbox = {
      .init = init,
      .update = update,
      .event = event,
  };
  return &sandbox;
}

const ClaymoreConfig *claymore_init(void) {
  static const ClaymoreConfig config = {
      .window =
          {
              .width = 720,
              .height = 420,
              .title = "sandbox",
          },
      .main = scene_init,
  };
  return &config;
}
