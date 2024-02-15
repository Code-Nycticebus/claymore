#ifndef __CLAYMORE_SCENE_H__
#define __CLAYMORE_SCENE_H__

#include "claymore/defines.h" // IWYU pragma: export
#include "claymore/event/types.h"

typedef struct {
  Arena arena;
  void *data;
} CmScene;

typedef struct {
  void (*init)(CmScene *scene);
  void (*update)(CmScene *scene, double deltatime);
  void (*free)(CmScene *scene);
  void (*on_event)(CmScene *scene, CmEvent *event);
} CmSceneInterface;

typedef CmSceneInterface *(*CmSceneInit)(void);

typedef struct CmSceneInternal {
  CmScene data; // has to be first!
  const CmSceneInterface *interface;
  struct CmSceneInternal *parent;
  DA(struct CmSceneInternal) children;
} CmSceneInternal;

void cm_scene_push(CmScene *scene, CmSceneInit init);

CmScene *cm_scene_parent(CmScene *scene);
void cm_scene_map_children(CmScene *scene, void (*map)(CmScene *, CmScene *));

// INTERNAL

CmSceneInternal cm_scene_internal_init(const CmSceneInit init);
void cm_scene_internal_update(CmSceneInternal *scene, double deltatime);
void cm_scene_internal_free(CmSceneInternal *scene);
void cm_scene_internal_on_event(CmSceneInternal *scene, CmEvent *event);

#endif /* !__CLAYMORE_SCENE_H__ */
