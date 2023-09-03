#include "claymore.h"

extern CmLayerInterface sandbox_cube(void);
extern CmLayerInterface sandbox_fps(void);

CmSceneInterface scene_cube(void) {
  return (CmSceneInterface){
      .init = NULL,
      .free = NULL,
      .layers =
          {
              sandbox_cube,
              sandbox_fps,
              NULL,
          },
  };
}
