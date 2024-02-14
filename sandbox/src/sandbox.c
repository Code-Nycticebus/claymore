#include "claymore/app/entrypoint.h"

#include "claymore/app/window.h"

#include "GLFW/glfw3.h"

static void sandbox2_init(CmScene *scene) {
  clib_log_info("sandbox2 init");
  scene->data = arena_alloc(&scene->arena, sizeof(bool));
  *(bool *)scene->data = false;
}

static void sandbox2_update(CmScene *scene) {
  bool *printed = scene->data;
  if (!*printed) {
    clib_log_info("sandbox2 update");
    *printed = true;
  }

  if (glfwGetKey(cm_window_context(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(cm_window_context(), true);
  }
}

static void sandbox2_free(CmScene *scene) {
  (void)scene;
  clib_log_info("sandbox2 free");
}

static CmSceneInterface *sandbox2_scene_init(void) {
  static CmSceneInterface sandbox2 = {
      .init = sandbox2_init,
      .update = sandbox2_update,
      .free = sandbox2_free,
  };
  return &sandbox2;
}

static void sandbox_init(CmScene *scene) {
  clib_log_info("sandbox init");
  scene->data = arena_alloc(&scene->arena, sizeof(bool));
  *(bool *)scene->data = false;

  cm_scene_push(scene, sandbox2_scene_init);
}

static void sandbox_update(CmScene *scene) {
  bool *printed = scene->data;
  if (!*printed) {
    clib_log_info("sandbox update");
    *printed = true;
  }

  if (glfwGetKey(cm_window_context(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(cm_window_context(), true);
  }
}

static void sandbox_free(CmScene *scene) {
  (void)scene;
  clib_log_info("sandbox free");
}

static CmSceneInterface *sandbox_scene_init(void) {
  static CmSceneInterface sandbox = {
      .init = sandbox_init,
      .update = sandbox_update,
      .free = sandbox_free,
  };
  return &sandbox;
}

ClaymoreConfig claymore_init(void) {
  return (ClaymoreConfig){
      .window = {.width = 720, .height = 420, .title = "sandbox"}, // NOLINT
      .scene = sandbox_scene_init,
  };
}
