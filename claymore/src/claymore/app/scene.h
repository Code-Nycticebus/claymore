#ifndef __CLAYMORE_SCENE_H__
#define __CLAYMORE_SCENE_H__

#include "claymore/defines.h" // IWYU pragma: export

#include "claymore/event/types.h"
#include "claymore/renderer/gpu.h"

typedef struct {
  Arena arena;
  CmGpu gpu;
  void *data;
} CmScene;

typedef const struct {
  void (*init)(CmScene *scene);
  void (*update)(CmScene *scene, double deltatime);
  void (*final)(CmScene *scene);
  void (*event)(CmScene *scene, CmEvent *event);
} CmSceneInterface;

typedef const CmSceneInterface *(*CmSceneInit)(void);

typedef struct CmSceneInternal {
  CmScene data; // has to be first!
  const CmSceneInterface *interface;
  struct CmSceneInternal *parent;
  DA(struct CmSceneInternal *) children;
} CmSceneInternal;

void cm_scene_push(CmScene *scene, CmSceneInit init);

CmScene *cm_scene_parent(CmScene *scene);
void cm_scene_map_children(CmScene *scene, void (*map)(CmScene *, CmScene *));

// INTERNAL

CmSceneInternal *cm_scene_internal_init(Arena *arena, CmSceneInit init);
void cm_scene_internal_update(CmSceneInternal *scene, double deltatime);
void cm_scene_internal_final(CmSceneInternal *scene);
void cm_scene_internal_event(CmSceneInternal *scene, CmEvent *event);

#endif /* !__CLAYMORE_SCENE_H__ */
