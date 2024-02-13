#ifndef __CLAYMORE_SCENE_H__
#define __CLAYMORE_SCENE_H__

#include "claymore/defines.h" // IWYU pragma: export

typedef struct CmScene CmScene;
typedef struct CmSceneInterface CmSceneInterface;
typedef struct CmSceneInternal CmSceneInternal;
typedef CmSceneInterface *(*CmSceneInit)(void);
typedef DA(CmSceneInit) CmSceneChildren;

struct CmScene {
  Arena arena;
  void *data;
};

struct CmSceneInterface {
  const char *name;
  void (*init)(CmScene *scene, CmSceneChildren *children);
  void (*update)(CmScene *scene);
  void (*free)(CmScene *scene);
};

struct CmSceneInternal {
  CmScene data; // has to be first!
  const CmSceneInterface *interface;
  DA(CmSceneInternal) children;
};

#endif /* !__CLAYMORE_SCENE_H__ */
