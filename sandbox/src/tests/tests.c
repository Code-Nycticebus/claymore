
#include "tests.h"

static bool test_init(CmScene *scene) {
  (void)scene;
  return true;
}

static void test_update(CmScene *scene, float dt) { (void)dt, (void)scene; }

static void test_free(CmScene *scene) { (void)scene; }

CmSceneInterface scene_test(void) {
  return (CmSceneInterface){
      .init = test_init,
      .update = test_update,
      .free = test_free,
      .layers = {0},
  };
}
