#include "noise/noise.h"

typedef struct {
  CmTexture texture;
} SceneData;

static bool noise_init(CmScene *scene) {
  SceneData *data = NULL;
  scene->state = data;
  return true;
}

static void noise_update(CmScene *scene, float dt) { (void)dt, (void)scene; }

static void noise_free(CmScene *scene) { (void)scene; }

CmSceneInterface scene_noise(void) {
  return (CmSceneInterface){
      .init = noise_init,
      .free = noise_free,
      .update = noise_update,
      .layers = {0},
  };
}
