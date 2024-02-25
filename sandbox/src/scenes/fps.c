#include "claymore.h"

typedef struct {
  float timer;
} Fps;

static void init(CmScene *scene) {
  Fps *fps = arena_calloc(&scene->arena, sizeof(Fps));
  scene->data = fps;
  fps->timer = 0;
}

static void update(CmScene *scene, double deltatime) {
  Fps *fps = scene->data;
  if ((fps->timer += deltatime) >= 1) {
    clib_log_info("%.0f fps", 1 / deltatime);
    fps->timer = 0;
  }
}

CmSceneInterface *fps_scene_init(void) {
  static CmSceneInterface sandbox = {
      .init = init,
      .update = update,
  };
  return &sandbox;
}
