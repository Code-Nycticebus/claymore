#include "claymore.h"

extern CmLayerInterface sandbox_ortho(void);
extern CmLayerInterface sandbox_background(void);
extern CmLayerInterface sandbox_fps(void);

CmSceneInterface scene_ortho(void) {
  return (CmSceneInterface){
      .init = NULL,
      .free = NULL,
      .layers =
          {
              sandbox_background,
              sandbox_ortho,
              sandbox_fps,
              NULL,
          },
  };
}
