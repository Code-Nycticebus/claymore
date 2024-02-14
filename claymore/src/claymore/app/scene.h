#ifndef __CLAYMORE_SCENE_H__
#define __CLAYMORE_SCENE_H__

#include "claymore/defines.h" // IWYU pragma: export

typedef struct CmScene CmScene;
typedef struct CmSceneInterface CmSceneInterface;
typedef struct CmSceneInternal CmSceneInternal;
typedef CmSceneInterface *(*CmSceneInit)(void);

struct CmScene {
  Arena arena;
  void *data;
};

struct CmSceneInterface {
  void (*init)(CmScene *scene);
  void (*update)(CmScene *scene);
  void (*free)(CmScene *scene);
};

struct CmSceneInternal {
  CmScene data; // has to be first!
  const CmSceneInterface *interface;
  DA(CmSceneInternal) children;
};

void cm_scene_push(CmScene *scene, CmSceneInit init);

CmSceneInternal cm_scene_internal_init(const CmSceneInit init);
void cm_scene_internal_update(CmSceneInternal *scene);
void cm_scene_internal_free(CmSceneInternal *scene);

#endif /* !__CLAYMORE_SCENE_H__ */
