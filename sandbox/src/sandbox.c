
#include <stdio.h>
#include <stdlib.h>

#include "claymore.h"

#define WINDOW_WIDTH 620
#define WINDOW_HEIGHT 420

extern CmLayer sandbox_layer(void);
extern CmLayer sandbox_overlay(void);

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
              NULL,
          },
  };
}
