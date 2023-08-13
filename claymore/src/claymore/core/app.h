#ifndef __CM_APP_H__
#define __CM_APP_H__

#include "camera.h"
#include "claymore/events/event.h"
#include "window.h"

typedef struct {
  mat4 projection;
  mat4 view;
} Camera;

typedef struct {
  CMwindow *window;
  bool run;
} CmApp;

#ifndef CM_LAYER_MAX
#define CM_LAYER_MAX 5
#endif

typedef struct {
  CmCamera camera;
  CmApp *app;
  void *state;
} CmLayer;

typedef struct {
  void (*init)(CmLayer *layer);
  void (*update)(CmLayer *layer, float dt);
  void (*free)(CmLayer *layer);
} CmLayerInterface;

typedef CmLayerInterface (*cm_layer_create_fn)(void);

typedef struct ClaymoreConfig {
  struct {
    uint32_t width;
    uint32_t height;
    const char *title;
  } window;

  cm_layer_create_fn layers[CM_LAYER_MAX];
} ClaymoreConfig;

bool cm_app_init(CmApp *app, const ClaymoreConfig *config);
void cm_app_shutdown(CmApp *app);

void cm_app_run(CmApp *app);

/* User functions! */
extern ClaymoreConfig claymore_config(void);

#endif /* !__CM_APP_H__ */
