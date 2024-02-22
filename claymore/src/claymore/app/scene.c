#include "scene.h"

void cm_scene_push(CmScene *scene, CmSceneInit init) {
  CmSceneInternal *internal = (CmSceneInternal *)scene;
  da_push(&internal->children, cm_scene_internal_init(init));
  da_last(&internal->children).parent = internal;
}

CmScene *cm_scene_parent(CmScene *scene) {
  CmSceneInternal *internal = (CmSceneInternal *)scene;
  return &internal->parent->data;
}

void cm_scene_map_children(CmScene *scene, void (*map)(CmScene *, CmScene *)) {
  CmSceneInternal *internal = (CmSceneInternal *)scene;
  for (usize i = 0; i < internal->children.len; i++) {
    map(scene, &internal->children.items[i].data);
  }
}

CmSceneInternal cm_scene_internal_init(const CmSceneInit init) {
  CmSceneInternal scene = {0};
  scene.interface = init();
  da_init(&scene.children, &scene.data.arena);
  scene.data.gpu = cm_gpu_internal_init(&scene.data.arena);
  scene.interface->init(&scene.data);
  return scene;
}

void cm_scene_internal_update(CmSceneInternal *scene, double deltatime) {
  for (usize i = 0; i < scene->children.len; i++) {
    cm_scene_internal_update(&scene->children.items[i], deltatime);
  }
  scene->interface->update(&scene->data, deltatime);
}

void cm_scene_internal_free(CmSceneInternal *scene) {
  for (usize i = 0; i < scene->children.len; i++) {
    cm_scene_internal_free(&scene->children.items[i]);
  }
  scene->interface->free(&scene->data);
  cm_gpu_internal_free(&scene->data.gpu);
  arena_free(&scene->data.arena);
}

void cm_scene_internal_on_event(CmSceneInternal *scene, CmEvent *event) {
  for (usize i = 0; i < scene->children.len; i++) {
    if (event->handled) {
      return;
    }
    cm_scene_internal_on_event(&scene->children.items[i], event);
  }
  if (scene->interface->on_event) {
    scene->interface->on_event(&scene->data, event);
  }
}
