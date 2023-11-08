#ifndef __CM_APP_DEFINES_H__
#define __CM_APP_DEFINES_H__

#include "camera.h"
#include "cm.h"
#include "window.h"

typedef struct {
  CMwindow *window;
  bool run;
  void *state;
} CmApp;

#ifndef CM_LAYER_MAX
#define CM_LAYER_MAX 5
#endif

typedef struct {
  CmCamera camera;
  void *state;
  bool active;
} CmLayer;

typedef struct {
  void *state;
  CmCamera camera;
  CmApp *app;
} CmScene;

typedef struct {
  bool (*init)(CmScene *scene, CmLayer *layer);
  void (*update)(CmScene *scene, CmLayer *layer, float dt);
  void (*free)(CmScene *scene, CmLayer *layer);
} CmLayerInterface;

typedef CmLayerInterface (*CmLayerCreateFn)(void);

typedef struct {
  bool (*init)(CmScene *);
  void (*update)(CmScene *, float);
  void (*free)(CmScene *);
  struct {
    CmLayerCreateFn layer;
    bool active;
  } layers[CM_LAYER_MAX];
} CmSceneInterface;

typedef CmSceneInterface (*CmSceneCreateFn)(void);

#ifndef CM_SCENES_MAX
#define CM_SCENES_MAX 5
#endif

typedef struct {
  bool (*init)(CmApp *);
  void (*update)(CmApp *, float);
  void (*free)(CmApp *);
} CmAppInterface;

typedef struct {
  struct {
    uint32_t width;
    uint32_t height;
    const char *title;
  } window;
  CmAppInterface app;
  CmSceneCreateFn scenes[CM_SCENES_MAX];
} ClaymoreConfig;

#endif /* !__CM_APP_DEFINES_H__ */
