#include "scene.h"

#include "claymore/app/app.h"

/* ========= scene public ========= */

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

void cm_scene_delete(CmScene *scene) {
  CmSceneInternal *parent = (CmSceneInternal *)cm_scene_parent(scene);
  for (usize i = 0; i < parent->children.len; i++) {
    if (da_get(&parent->children, i) == (CmSceneInternal *)scene) {
      da_remove(&parent->children, i);
      break;
    }
  }
  cm_app_internal_schedule_delete(scene);
}

void *cm_scene_data(CmScene *scene) {
  CmSceneInternal *internal = (CmSceneInternal *)scene;
  return internal->data;
}

Str cm_scene_name(CmScene *scene) {
  CmSceneInternal *internal = (CmSceneInternal *)scene;
  return internal->interface->name;
}

CmScene *cm_scene_parent(CmScene *scene) {
  CmSceneInternal *internal = (CmSceneInternal *)scene;
  return &internal->parent->public;
}

CmScene *cm_scene_child(CmScene *scene, usize idx) {
  CmSceneInternal *internal = (CmSceneInternal *)scene;
  return (CmScene *)internal->children.items[idx];
}

const CmSceneDa *cm_scene_children(CmScene *scene) {
  CmSceneInternal *internal = (CmSceneInternal *)scene;
  return (CmSceneDa *)&internal->children;
}

CmScene *cm_scene_find(CmScene *root, Str name) {
  CmSceneInternal *internal = (CmSceneInternal *)root;
  for (usize i = 0; i < da_len(&internal->children); ++i) {
    CmSceneInternal *scene = da_get(&internal->children, i);
    if (str_eq(scene->interface->name, name)) {
      return (CmScene *)scene;
    }
  }
  return NULL;
}

void cm_scene_find_all(CmSceneDa *out, CmScene *root, Str name) {
  CmSceneInternal *internal = (CmSceneInternal *)root;
  for (usize i = 0; i < da_len(&internal->children); ++i) {
    CmSceneInternal *scene = da_get(&internal->children, i);
    if (str_eq(scene->interface->name, name)) {
      da_push(out, (CmScene *)scene);
    }
  }
}

/* ========= scene internal ========= */

CmSceneInternal *cm_scene_internal_init(Arena *arena, const CmSceneInit init) {
  CmSceneInterface *interface = init();
  usize size = sizeof(CmSceneInternal) + interface->size;
  CmSceneInternal *scene = arena_calloc_chunk(arena, size);

  scene->interface = interface;
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
