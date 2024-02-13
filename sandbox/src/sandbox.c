#include "claymore/app/entrypoint.h"

#include "claymore/app/window.h"

#include "GLFW/glfw3.h"

static void sandbox_init(CmScene *scene, CmSceneChildren *children) {
  (void)children;
  clib_log_info("sandbox init");
  scene->data = arena_alloc(&scene->arena, sizeof(bool));
  *(bool *)scene->data = false;
}

static void sandbox_update(CmScene *scene) {
  bool *printed = scene->data;
  if (!*printed) {
    clib_log_info("sandbox update");
    *printed = true;
  }

  if (glfwGetKey(cm_window_context(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(cm_window_context(), true);
}

static void sandbox_free(CmScene *scene) {
  (void)scene;
  clib_log_info("sandbox free");
}

static CmSceneInterface *sandbox_scene_init(void) {
  static CmSceneInterface sandbox = {
      .name = "sandbox",
      .init = sandbox_init,
      .update = sandbox_update,
      .free = sandbox_free,
  };
  return &sandbox;
}

ClaymoreConfig claymore_init(void) {
  return (ClaymoreConfig){
      .window = {.width = 720, .height = 420, .title = "sandbox"},
      .scene = sandbox_scene_init,
  };
}
