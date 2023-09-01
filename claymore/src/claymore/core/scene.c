#include "scene.h"
#include "claymore/core/app_defines.h"
#include "claymore/events/event.h"

typedef struct {
  size_t current;
  CmApp *app;
  const ClaymoreConfig *config;
  CmSceneInterface interface;
  CmScene scene;
  struct {
    size_t count;
    CmLayerInterface interace[CM_LAYER_MAX];
    CmLayer layer[CM_LAYER_MAX];
  } layer_stack;
} SceneManager;

static SceneManager scene_manager;

static bool _cm_scene_init(size_t scene) {
  scene_manager.interface = scene_manager.config->scenes[scene]();
  scene_manager.scene.app = scene_manager.app;
  if (scene_manager.interface.init) {
    if (!scene_manager.interface.init(&scene_manager.scene)) {
      return false;
    }
  }

  for (size_t i = 0; i < CM_LAYER_MAX; i++) {
    if (scene_manager.interface.layers[i] == NULL) {
      break;
    }
    scene_manager.layer_stack.interace[i] = scene_manager.interface.layers[i]();
    assert(scene_manager.layer_stack.interace[i].init);
    if (!scene_manager.layer_stack.interace[i].init(
            &scene_manager.scene, &scene_manager.layer_stack.layer[i])) {
      return false;
    }
    scene_manager.layer_stack.count++;
  }
  return true;
}

bool cm_scene_init(CmApp *app, const ClaymoreConfig *config) {
  assert(config->scenes[0] != NULL);
  scene_manager.current = 0;
  scene_manager.app = app;
  scene_manager.config = config;
  _cm_scene_init(scene_manager.current);

  return true;
}

void cm_scene_update(float dt) {
  cm_camera_update(&scene_manager.scene.camera);
  for (size_t i = 0; i < scene_manager.layer_stack.count; ++i) {
    cm_camera_update(&scene_manager.layer_stack.layer[i].camera);
    scene_manager.layer_stack.interace[i].update(
        &scene_manager.scene, &scene_manager.layer_stack.layer[i], dt);
  }
}

static void _cm_scene_free(void) {
  for (size_t i = 0; i < scene_manager.layer_stack.count; i++) {
    scene_manager.layer_stack.interace[i].free(
        &scene_manager.scene, &scene_manager.layer_stack.layer[i]);
  }
  if (scene_manager.interface.free) {
    scene_manager.interface.free(&scene_manager.scene);
  }
  cm_event_top_reset();
  scene_manager.layer_stack.count = 0;
}

void cm_scene_free(void) { _cm_scene_free(); }

void cm_scene_change(size_t scene) {
  assert(scene < CM_SCENES_MAX);
  if (scene != scene_manager.current) {
    _cm_scene_free();
    scene_manager.current = scene;
    _cm_scene_init(scene);
  }
}
