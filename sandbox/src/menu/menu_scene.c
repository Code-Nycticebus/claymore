#include "claymore.h"

extern CmLayerInterface sandbox_menu(void);

CmSceneInterface scene_menu(void) {
  return (CmSceneInterface){
      .init = NULL,
      .free = NULL,
      .update = NULL,
      .layers =
          {
              sandbox_menu,
              NULL,
          },
  };
}
