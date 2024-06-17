#include "scene.h"

#include "claymore/app/app.h"

CmScene *cm_scene_push(CmScene *scene, CmSceneInit init) {
  CmSceneInternal *internal = (CmSceneInternal *)scene;
  CmSceneInternal *new = cm_scene_internal_init(&internal->arena, init);
  new->parent = internal;
  if (new->interface->init) {
    new->interface->init(&new->public);
  }
  da_push(&internal->children, new);
  cm_app_internal_schedule_build();
  return (CmScene *)new;
}

void cm_scene_delete(CmScene *parent, CmScene *scene) {
  CmSceneInternal *internal = (CmSceneInternal *)parent;
  for (usize i = 0; i < internal->children.len; i++) {
    if (da_get(&internal->children, i) == (CmSceneInternal *)scene) {
      cm_app_internal_schedule_delete(scene);
      da_remove(&internal->children, i);
      return;
    }
  }
  cebus_assert_debug(false, "Scene not found!");
  UNREACHABLE();
}

void cm_scene_delete_self(CmScene *scene) {
  CmScene *parent = cm_scene_parent(scene);
  cm_scene_delete(parent, scene);
}

CmScene *cm_scene_parent(CmScene *scene) {
  CmSceneInternal *internal = (CmSceneInternal *)scene;
  return &internal->parent->public;
}

void cm_scene_map_children(CmScene *scene, void (*map)(CmScene *, CmScene *)) {
  CmSceneInternal *internal = (CmSceneInternal *)scene;
  for (usize i = 0; i < internal->children.len; i++) {
    map(scene, &internal->children.items[i]->public);
  }
}

void *cm_scene_set_data(CmScene *scene, usize size) {
  cebus_assert(scene->data == NULL, "Trying to set data twice");
  scene->data = arena_calloc(&((CmSceneInternal *)scene)->arena, size);
  return scene->data;
}

const CmSceneChildren *cm_scene_children(CmScene *scene) {
  CmSceneInternal *internal = (CmSceneInternal *)scene;
  return (CmSceneChildren *)&internal->children;
}

CmSceneInternal *cm_scene_internal_init(Arena *arena, const CmSceneInit init) {
  CmSceneInternal *scene = arena_calloc_chunk(arena, sizeof(CmSceneInternal));
  scene->interface = init();
  da_init(&scene->children, &scene->arena);
  scene->public.gpu = cm_gpu_internal_init(&scene->arena);
  return scene;
}

void cm_scene_internal_final(Arena *arena, CmSceneInternal *scene) {
  for (usize i = 0; i < scene->children.len; i++) {
    cm_scene_internal_final(&scene->arena, scene->children.items[i]);
  }
  if (scene->interface->final) {
    scene->interface->final(&scene->public);
  }
  cm_gpu_internal_free(&scene->public.gpu);
  arena_free(&scene->public.arena);
  arena_free(&scene->arena);
  arena_free_chunk(arena, scene);
}

void cm_scene_internal_event(CmSceneInternal *scene, CmEvent *event) {
  for (usize i = 0; i < scene->children.len; i++) {
    if (event->handled) {
      return;
    }
    cm_scene_internal_event(scene->children.items[i], event);
  }
  if (scene->interface->event) {
    scene->interface->event(&scene->public, event);
  }
}
