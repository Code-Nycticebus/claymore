#include "claymore/entrypoint.h"

CmSceneInterface *fps_scene_init(void);

typedef struct {
  float zoom;
  vec3 position;
  mat4 view;
  mat4 projection;
} Camera;

typedef struct {
  usize count;
  Camera camera;
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
  sandbox->count = 0;
  cm_scene_push(scene, fps_scene_init);
}

static CmSceneInterface *scene_init(void) {
  static CmSceneInterface sandbox = {
      .init = init,
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
