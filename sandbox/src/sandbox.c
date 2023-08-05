
#include <stdio.h>
#include <stdlib.h>

#include "claymore.h"

#define WINDOW_WIDTH 620
#define WINDOW_HEIGHT 420

extern CmLayerInterface sandbox_layer(void);
extern CmLayerInterface sandbox_overlay(void);
extern CmLayerInterface sandbox_fps(void);

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
              sandbox_layer,
              sandbox_overlay,
              sandbox_fps,
          },
  };
}
