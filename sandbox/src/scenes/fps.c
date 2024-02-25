#include "claymore.h"

static void init(CmScene *scene) {
  float *timer = arena_alloc(&scene->arena, sizeof(float));
  *timer = 0;
  scene->data = timer;
}

static void update(CmScene *scene, double deltatime) {
  float *timer = scene->data;
  if ((*timer += deltatime) >= 1) {
    clib_log_info("%.0f fps", 1 / deltatime);
    *timer = 0;
  }
}

CmSceneInterface *fps_scene_init(void) {
  static CmSceneInterface sandbox = {
      .init = init,
      .update = update,
  };
  return &sandbox;
}
