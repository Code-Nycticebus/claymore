#ifndef __CLAYMORE_SCENE_H__
#define __CLAYMORE_SCENE_H__

#include "claymore/defines.h" // IWYU pragma: export

typedef struct {
  Arena arena;
  void *data;
} CmScene;

typedef struct {
  void (*init)(CmScene *scene);
  void (*update)(CmScene *scene, double deltatime);
  void (*free)(CmScene *scene);
} CmSceneInterface;

typedef CmSceneInterface *(*CmSceneInit)(void);

typedef struct CmSceneInternal {
  CmScene data; // has to be first!
  const CmSceneInterface *interface;
  DA(struct CmSceneInternal) children;
} CmSceneInternal;

void cm_scene_push(CmScene *scene, CmSceneInit init);

CmSceneInternal cm_scene_internal_init(const CmSceneInit init);
void cm_scene_internal_update(CmSceneInternal *scene, double deltatime);
void cm_scene_internal_free(CmSceneInternal *scene);

#endif /* !__CLAYMORE_SCENE_H__ */
