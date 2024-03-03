#include "scene.h"

void cm_scene_push(CmScene *scene, CmSceneInit init) {
  CmSceneInternal *internal = (CmSceneInternal *)scene;
  CmSceneInternal *new = cm_scene_internal_init(&scene->arena, init);
  new->parent = internal;
  if (new->interface->init) {
    new->interface->init(&new->data);
  }
  da_push(&internal->children, new);
}

CmScene *cm_scene_parent(CmScene *scene) {
  CmSceneInternal *internal = (CmSceneInternal *)scene;
  return &internal->parent->data;
}

void cm_scene_map_children(CmScene *scene, void (*map)(CmScene *, CmScene *)) {
  CmSceneInternal *internal = (CmSceneInternal *)scene;
  for (usize i = 0; i < internal->children.len; i++) {
    map(scene, &internal->children.items[i]->data);
  }
}

void *cm_scene_alloc_data(CmScene *scene, usize size) {
  scene->data = arena_calloc(&scene->arena, size);
  return scene->data;
}

CmSceneInternal *cm_scene_internal_init(Arena *arena, const CmSceneInit init) {
  CmSceneInternal *scene = arena_calloc(arena, sizeof(CmSceneInternal));
  scene->interface = init();
  da_init(&scene->children, &scene->data.arena); // NOLINT
  scene->data.gpu = cm_gpu_internal_init(&scene->data.arena);
  return scene;
}

void cm_scene_internal_update(CmSceneInternal *scene, double deltatime) {
  for (usize i = 0; i < scene->children.len; i++) {
    cm_scene_internal_update(scene->children.items[i], deltatime);
  }
  if (scene->interface->update) {
    scene->interface->update(&scene->data, deltatime);
  }
}

void cm_scene_internal_final(CmSceneInternal *scene) {
  for (usize i = 0; i < scene->children.len; i++) {
    cm_scene_internal_final(scene->children.items[i]);
  }
  if (scene->interface->final) {
    scene->interface->final(&scene->data);
  }
  cm_gpu_internal_free(&scene->data.gpu);
  arena_free(&scene->data.arena);
}

void cm_scene_internal_event(CmSceneInternal *scene, CmEvent *event) {
  for (usize i = 0; i < scene->children.len; i++) {
    if (event->handled) {
      return;
    }
    cm_scene_internal_event(scene->children.items[i], event);
  }
  if (scene->interface->event) {
    scene->interface->event(&scene->data, event);
  }
}
