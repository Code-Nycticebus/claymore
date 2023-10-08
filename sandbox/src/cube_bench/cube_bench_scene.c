#include "claymore.h"

extern CmLayerInterface sandbox_cube_bench(void);
extern CmLayerInterface sandbox_fps(void);

CmSceneInterface scene_cube_bench(void) {
  return (CmSceneInterface){
      .init = NULL,
      .free = NULL,
      .update = NULL,
      .layers =
          {
              sandbox_cube_bench,
              sandbox_fps,
              NULL,
          },
  };
}
