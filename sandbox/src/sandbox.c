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
  Sandbox *sandbox = cm_scene_alloc_data(scene, sizeof(Sandbox));
  (void)sandbox;

  cm_scene_push(scene, fps_scene_init);
}

static void update(CmScene *scene, double dt) {
  (void)dt;
  Sandbox *sandbox = scene->data;
  mat4 vp = GLM_MAT4_IDENTITY_INIT;
  (void)sandbox, (void)vp;
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
