#include "claymore.h"

typedef struct {
  float timer;
  usize id;
} Fps;

static void init(CmScene *scene) {
  Fps *fps = arena_calloc(&scene->arena, sizeof(Fps));
  scene->data = fps;
  fps->timer = 0;

  usize *count = cm_scene_parent(scene)->data;
  clib_log_info("Count: %" USIZE_FMT, *count);
  fps->id = *count;
  (*count)++;
}

static void update(CmScene *scene, double deltatime) {
  Fps *fps = scene->data;
  if ((fps->timer += deltatime) >= 1) {
    clib_log_info("%" USIZE_FMT ": %.0f fps", fps->id, 1 / deltatime);
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
