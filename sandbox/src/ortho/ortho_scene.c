#include "claymore.h"

extern CmLayerInterface sandbox_ortho(void);
extern CmLayerInterface sandbox_background(void);
extern CmLayerInterface sandbox_fps(void);

// bool ortho_scene_init(CmScene* scene) {
//   cm_layer_push(sandbox_background);
// }

CmSceneInterface scene_ortho(void) {
  return (CmSceneInterface){
      .init = NULL,
      .free = NULL,
      .update = NULL,
      .layers =
          {
              {sandbox_background, true},
              {sandbox_ortho, true},
              {sandbox_fps, true},
              {0},
          },
  };
}
