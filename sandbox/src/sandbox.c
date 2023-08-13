#include "claymore.h"

#define WINDOW_WIDTH 620
#define WINDOW_HEIGHT 420

extern CmLayerInterface sandbox_cube(void);
extern CmLayerInterface sandbox_gui(void);
extern CmLayerInterface sandbox_ortho(void);
extern CmLayerInterface sandbox_fps(void);
extern CmLayerInterface sandbox_batch_test(void);

ClaymoreConfig claymore_config(void) {
  return (const ClaymoreConfig){
      .window =
          {
              .width = WINDOW_WIDTH,
              .height = WINDOW_HEIGHT,
              .title = "Sandbox",
          },
      .layers =
          {
              sandbox_ortho,
              // sandbox_cube,
              sandbox_gui,
              sandbox_fps,
              NULL,
          },
  };
}
