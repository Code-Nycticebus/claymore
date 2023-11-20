
#include "tests.h"

static CmMesh mesh;
static CmShader shader;

static bool test_init(CmScene *scene) {
  scene->camera = cm_camera_screen_init((vec3s){0}, scene->app->window->width,
                                        scene->app->window->height);
  shader = cm_shader_load_from_file("res/shader/basic.vs.glsl",
                                    "res/shader/basic.fs.glsl");
  return true;
}

static void test_update(CmScene *scene, float dt) {
  (void)dt;
  cm_shader_bind(&shader);
  cm_shader_set_mat4(&shader, "u_mvp", scene->camera.vp);
  const vec3s points[] = {
      {{100.F, 100.F, 0.F}}, {{100.F, 200.F, 0.F}}, {{200.F, 300.F, 0.F}},
      {{200.F, 340.F, 0.F}}, {{400.F, 300.F, 0.F}},
  };
  const size_t points_count = sizeof(points) / sizeof(points[0]);
  cm_renderer_lines_begin();
  for (size_t i = 0; i < points_count; i++) {
    glLineWidth(20.F);
    cm_renderer_lines_push(points[i], (vec4s){{1, 1, 1, 1}});
  }
  cm_renderer_lines_end();
  cm_shader_unbind();
}

static void test_free(CmScene *scene) {
  (void)scene;
  cm_mesh_delete(&mesh);
}

CmSceneInterface scene_test(void) {
  return (CmSceneInterface){
      .init = test_init,
      .update = test_update,
      .free = test_free,
      .layers = {0},
  };
}
