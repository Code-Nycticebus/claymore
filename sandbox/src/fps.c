#include "claymore.h"
#include "claymore/events/event.h"
#include "claymore/events/mouse.h"
#include <GL/gl.h>
#include <stdio.h>

static void fps_init(CmLayer *layer) { (void)layer; }

static void fps_update(CmLayer *layer, float dt) { (void)layer, (void)dt; }

static void fps_free(CmLayer *layer) { (void)layer; }

CmLayerInterface sandbox_fps(void) {
  return (CmLayerInterface){
      .init = fps_init,
      .free = fps_free,
      .update = fps_update,
  };
}
