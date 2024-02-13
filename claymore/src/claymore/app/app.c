#include "app.h"

#include "renderer/context.h"
#include "window.h"

#include "GL/glew.h"

typedef struct {
  CmSceneInterface *scene;
} CmApp;

static CmApp app;

CmSceneInternal main_scene;

static CmSceneInternal cm_scene_init(const CmSceneInterface *interface) {
  CmSceneInternal scene = {0};
  scene.interface = interface;

  Arena temp = {0};
  CmSceneChildren children = {0};
  da_init(&children, &temp);
  scene.interface->init(&scene.data, &children);

  if (0 < children.len) {
    da_init(&scene.children, &scene.data.arena);
    for (usize i = 0; i < children.len; ++i) {
      da_push(&scene.children, cm_scene_init(children.items[i]()));
    }
  }

  arena_free(&temp);
  return scene;
}

static void cm_scene_update(CmSceneInternal *scene) {
  scene->interface->update(&scene->data);
  for (usize i = 0; i < scene->children.len; i++) {
    CmSceneInternal *child = &scene->children.items[i];
    cm_scene_update(child);
  }
}

static void cm_scene_free(CmSceneInternal *scene) {
  for (usize i = 0; i < scene->children.len; i++) {
    CmSceneInternal *child = &scene->children.items[i];
    cm_scene_free(child);
  }
  scene->interface->free(&scene->data);
  arena_free(&scene->data.arena);
}

bool app_init(const ClaymoreConfig *config) {
  if (!cm_window_create(config->window.width, config->window.height,
                        config->window.title)) {
    clib_log_error("Could not open window");
    return false;
  }

  cm_platform_context_init(cm_window_context());

  glClearColor(1, 0, 0, 1);

  app.scene = config->scene();
  main_scene = cm_scene_init(app.scene);

  return true;
}

bool app_update(void) {
  if (cm_window_should_close()) {
    return false;
  }
  glClear(GL_COLOR_BUFFER_BIT);

  // RENDER HERE
  cm_scene_update(&main_scene);

  cm_window_swap_buffers();
  cm_window_poll_events();
  return true;
}

void app_terminate(void) {
  cm_scene_free(&main_scene);

  cm_window_close();
}
