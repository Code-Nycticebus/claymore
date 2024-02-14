#include "scene.h"

void cm_scene_push(CmScene *scene, CmSceneInit init) {
  CmSceneInternal *scene_internal = (CmSceneInternal *)scene;
  da_push(&scene_internal->children, cm_scene_internal_init(init));
}

CmSceneInternal cm_scene_internal_init(const CmSceneInit init) {
  CmSceneInternal scene = {0};
  scene.interface = init();
  da_init(&scene.children, &scene.data.arena);
  scene.interface->init(&scene.data);
  return scene;
}

void cm_scene_internal_update(CmSceneInternal *scene) {
  scene->interface->update(&scene->data);
  for (usize i = 0; i < scene->children.len; i++) {
    cm_scene_internal_update(&scene->children.items[i]);
  }
}

void cm_scene_internal_free(CmSceneInternal *scene) {
  for (usize i = 0; i < scene->children.len; i++) {
    cm_scene_internal_free(&scene->children.items[i]);
  }
  scene->interface->free(&scene->data);
  arena_free(&scene->data.arena);
}
