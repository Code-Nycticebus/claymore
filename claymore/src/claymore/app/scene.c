#define CLAYMORE_SCENE_INTERNAL
#include "scene.h"

void cm_scene_push(CmScene *scene, CmSceneInit init) {
  da_push(&((CmSceneInternal *)scene)->children, cm_scene_internal_init(init));
}

CmSceneInternal cm_scene_internal_init(const CmSceneInit init) {
  CmSceneInternal scene = {0};
  scene.interface = init();
  da_init(&scene.children, &scene.data.arena);
  scene.interface->init(&scene.data);
  return scene;
}

void cm_scene_internal_update(CmSceneInternal *scene, double deltatime) {
  scene->interface->update(&scene->data, deltatime);
  for (usize i = 0; i < scene->children.len; i++) {
    cm_scene_internal_update(&scene->children.items[i], deltatime);
  }
}

void cm_scene_internal_free(CmSceneInternal *scene) {
  for (usize i = 0; i < scene->children.len; i++) {
    cm_scene_internal_free(&scene->children.items[i]);
  }
  scene->interface->free(&scene->data);
  arena_free(&scene->data.arena);
}

void cm_scene_internal_on_event(CmSceneInternal *scene, CmEvent *event) {
  if (scene->interface->on_event) {
    scene->interface->on_event(&scene->data, event);
  }
  for (usize i = 0; i < scene->children.len; i++) {
    if (event->handled) {
      return;
    }
    cm_scene_internal_on_event(&scene->children.items[i], event);
  }
}
