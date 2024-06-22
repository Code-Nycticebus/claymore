#ifndef __CLAYMORE_SCENE_H__
#define __CLAYMORE_SCENE_H__

#include "claymore/defines.h" // IWYU pragma: export

#include "claymore/app/event.h"
#include "claymore/renderer/gpu.h"

typedef struct {
  Arena arena;
  CmGpu gpu;
} CmScene;

typedef const struct {
  usize size;

  void (*init)(CmScene *scene);
  void (*fixed_update)(CmScene *scene);

  void (*frame_update)(CmScene *scene);
  void (*pre_update)(CmScene *scene);
  void (*post_update)(CmScene *scene);

  void (*final)(CmScene *scene);
  void (*event)(CmScene *scene, CmEvent *event);
} CmSceneInterface;

typedef const CmSceneInterface *(*CmSceneInit)(void);

CmScene *cm_scene_push(CmScene *scene, CmSceneInit init);
void cm_scene_delete(CmScene *parent, CmScene *scene);
void cm_scene_delete_self(CmScene *scene);

CmScene *cm_scene_parent(CmScene *scene);
void cm_scene_map_children(CmScene *scene, void (*map)(CmScene *, CmScene *));
void *cm_scene_data(CmScene *scene);

typedef DA(CmScene *) CmSceneChildren;
const CmSceneChildren *cm_scene_children(CmScene *scene);

// INTERNAL

typedef struct CmSceneInternal {
  CmScene public; // has to be first!
  Arena arena;
  const CmSceneInterface *interface;
  struct CmSceneInternal *parent;
  DA(struct CmSceneInternal *) children;
  CGLM_ALIGN_MAT u8 data[];
} CmSceneInternal;

CmSceneInternal *cm_scene_internal_init(Arena *arena, CmSceneInit init);

void cm_scene_internal_final(Arena *arena, CmSceneInternal *scene);
void cm_scene_internal_event(CmSceneInternal *scene, CmEvent *event);

#endif /* !__CLAYMORE_SCENE_H__ */
