#include "claymore/entrypoint.h"

#include "claymore.h"

const usize height = 420;
const usize width = 720;
const float aspect = (float)width / (float)height;

typedef struct {
  float zoom;
  vec3 position;
  mat4 view;
  mat4 projection;
} Camera;

typedef struct {
  Camera camera;
} Sandbox;

static void event(CmScene *scene, CmEvent *event) {
  Sandbox *sandbox = scene->data;
  (void)sandbox, (void)event;
}

static void init(CmScene *scene) {
  clib_log_info("sandbox init");
  Sandbox *sandbox = arena_alloc(&scene->arena, sizeof(Sandbox));
  scene->data = sandbox;
}

static void update(CmScene *scene, double deltatime) {
  static double timer = 0;
  if ((timer += deltatime) >= 1) {
    clib_log_info("fps: %.0f", 1 / deltatime);
    timer = 0;
  }
  Sandbox *sandbox = scene->data;
  (void)sandbox;
}

static void final(CmScene *scene) {
  (void)scene;
  clib_log_info("sandbox final");
}

static CmSceneInterface *scene_init(void) {
  static CmSceneInterface sandbox = {
      .init = init,
      .update = update,
      .final = final,
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
      .scene = scene_init,
  };
  return &config;
}
