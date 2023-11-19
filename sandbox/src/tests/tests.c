
#include "tests.h"

static CmMesh mesh;
static CmShader shader;

static bool test_init(CmScene *scene) {
  vec3s points[] = {
      {{100.F, 100.F, 0.F}},
      {{100.F, 200.F, 0.F}},
      {{200.F, 100.F, 0.F}},
      {{400.F, 300.F, 0.F}},
  };
  mesh = cm_mesh_create(points, 4);
  vec4s colors[] = {
      {{1, 1, 1, 1}},
      {{1, 1, 1, 1}},
      {{1, 1, 1, 1}},
      {{1, 1, 1, 1}},
  };
  cm_mesh_attach_colors(&mesh, colors, 4);
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
  glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo.positions);
  glDrawArrays(GL_LINE_STRIP, 0, 4);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
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
