#include "claymore/app/entrypoint.h"

static void sandbox_init(CmScene *scene, CmSceneChildren *children) {
  (void)children;
  clib_log_info("INIT SANDBOX");
  scene->data = arena_alloc(&scene->arena, sizeof(bool));
  *(bool *)scene->data = false;
}

static void sandbox_update(CmScene *scene) {
  bool *printed = scene->data;
  if (!*printed) {
    clib_log_info("SANDBOX UPDATE");
    *printed = true;
  }
}

static void sandbox_free(CmScene *scene) {
  (void)scene;
  clib_log_info("sandbox free");
}

static CmSceneInterface sandbox = {
    .name = "sandbox",
    .init = sandbox_init,
    .update = sandbox_update,
    .free = sandbox_free,
};

ClaymoreConfig claymore_init(void) {
  return (ClaymoreConfig){
      .window = {.width = 720, .height = 420, .title = "sandbox"},
      .scene = &sandbox,
  };
}
