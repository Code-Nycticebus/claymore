#include "models.h"

typedef struct {
  CmMesh model;
  CmShader shader;
} SceneData;

static bool models_init(CmScene *scene) {
  SceneData *data = malloc(sizeof(SceneData));

  const float fov = 90.F;
  scene->camera = cm_camera_init_perspective(
      (vec3s){{0, 0, 4}}, (vec3s){0}, fov,
      (float)scene->app->window->width / (float)scene->app->window->height);

  // cube_shader = cm_shader_load_from_file("res/shader/cube.vs.glsl",
  //                                        "res/shader/cube.fs.glsl");
  data->shader = cm_shader_load_from_file("res/shader/uniform.vs.glsl",
                                          "res/shader/uniform.fs.glsl");

  data->model = cm_model_load("res/models/monkey.obj");

  camera_register_callbacks(&scene->camera);

  scene->state = data;
  return true;
}

static void models_free(CmScene *scene) { free(scene->state); }

static void models_update(CmScene *scene, float dt) {
  (void)dt;
  SceneData *data = scene->state;

  cm_shader_bind(&data->shader);
  cm_shader_set_mat4(&data->shader, "u_mvp", scene->camera.vp);
  cm_shader_set_vec4(&data->shader, "u_color", (vec4s){{1, 0, 0, 1}});

  // RENDER HERE
  cm_mesh_draw(&data->model);

  cm_shader_unbind();
}

CmSceneInterface scene_models(void) {
  return (CmSceneInterface){
      .init = models_init,
      .free = models_free,
      .update = models_update,
      .layers = {0},
  };
}
