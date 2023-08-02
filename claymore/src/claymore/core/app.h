#ifndef __CM_APP_H__
#define __CM_APP_H__

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

#define CM_LAYER_MAX 3

typedef struct {
  void *state;
  Camera camera;
  CmApp *app;
} CmLayerData;

typedef struct {
  bool (*init)(CmLayerData *layer);
  bool (*update)(CmLayerData *layer, float dt);
  bool (*free)(CmLayerData *layer);

  CmLayerData data;
} CmLayer;

typedef CmLayer (*cm_layer_create)(void);

typedef struct ClaymoreConfig {
  struct {
    uint32_t width;
    uint32_t height;
    const char *title;
  } window;

  cm_layer_create layers[CM_LAYER_MAX];
} ClaymoreConfig;

bool cm_app_init(CmApp *app, const ClaymoreConfig *config);
void cm_app_shutdown(CmApp *app);

/* User functions! */
extern ClaymoreConfig claymore_config(void);

#endif /* !__CM_APP_H__ */
