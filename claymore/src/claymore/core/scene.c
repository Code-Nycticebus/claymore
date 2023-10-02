#include "scene.h"
#include "claymore/core/app_defines.h"
#include "claymore/events/event.h"

typedef struct {
  size_t current;
  size_t count;
  CmApp *app;
  const ClaymoreConfig *config;
  CmSceneInterface interface;
  CmScene scene;

} SceneManager;

static SceneManager scene_manager;

typedef struct {
  size_t count;
  struct {
    CmLayerInterface interface;
    CmLayer layer;
  } stack[CM_LAYER_MAX];
} LayerStack;

static LayerStack layer_stack;

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
    layer_stack.stack[i].interface = scene_manager.interface.layers[i]();
    assert(layer_stack.stack[i].interface.init);
    if (!layer_stack.stack[i].interface.init(&scene_manager.scene,
                                             &layer_stack.stack[i].layer)) {
      return false;
    }
    layer_stack.count++;
  }
  return true;
}

bool cm_scene_init(CmApp *app, const ClaymoreConfig *config) {
  assert(config->scenes[0] != NULL);
  scene_manager.current = 0;
  scene_manager.app = app;
  scene_manager.config = config;
  for (size_t i = 0; i < CM_SCENES_MAX; i++) {
    if (config->scenes[i] == NULL) {
      break;
    }
    scene_manager.count++;
  }

  _cm_scene_init(scene_manager.current);

  return true;
}

void cm_scene_update(float dt) {
  cm_camera_update(&scene_manager.scene.camera);
  if (scene_manager.interface.update) {
    scene_manager.interface.update(&scene_manager.scene, dt);
  }
  for (size_t i = 0; i < layer_stack.count; ++i) {
    cm_camera_update(&layer_stack.stack[i].layer.camera);
    layer_stack.stack[i].interface.update(&scene_manager.scene,
                                          &layer_stack.stack[i].layer, dt);
  }
}

static void _cm_scene_free(void) {
  for (size_t i = 0; i < layer_stack.count; i++) {
    layer_stack.stack[i].interface.free(&scene_manager.scene,
                                        &layer_stack.stack[i].layer);
  }
  if (scene_manager.interface.free) {
    scene_manager.interface.free(&scene_manager.scene);
  }
  cm_event_top_reset();
  layer_stack.count = 0;
}

void cm_scene_free(void) { _cm_scene_free(); }

void cm_scene_change(size_t scene) {
  assert(scene < CM_SCENES_MAX);
  assert(scene < scene_manager.count);
  if (scene != scene_manager.current) {
    _cm_scene_free();
    scene_manager.current = scene;
    _cm_scene_init(scene);
  }
}
