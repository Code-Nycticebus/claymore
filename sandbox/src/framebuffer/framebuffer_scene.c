#include "claymore.h"

extern CmLayerInterface sandbox_framebuffer(void);

CmSceneInterface scene_framebuffer(void) {
  return (CmSceneInterface){
      .init = NULL,
      .free = NULL,
      .layers =
          {
              sandbox_framebuffer,
              NULL,
          },
  };
}
