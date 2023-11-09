#include "models.h"

#include "camera.h"

typedef struct {
  CmMesh model;
  CmShader shader;
} SceneData;

typedef struct {
  vec3s ambient;
  vec3s diffuse;
  vec3s specular;
  float shininess;
} Material;

void material_set(CmShader *shader, const Material *material) {
  cm_shader_set_vec3(shader, "material.ambient", material->ambient);
  cm_shader_set_vec3(shader, "material.diffuse", material->diffuse);
  cm_shader_set_vec3(shader, "material.specular", material->specular);
  cm_shader_set_f32(shader, "material.shininess", material->shininess);
}

static bool models_init(CmScene *scene) {
  SceneData *data = malloc(sizeof(SceneData));

  const float fov = 90.F;
  scene->camera = cm_camera_init_perspective(
      (vec3s){{0, 0, 4}}, (vec3s){0}, fov,
      (float)scene->app->window->width / (float)scene->app->window->height);

  data->shader = cm_shader_load_from_file("res/shader/material.vs.glsl",
                                          "res/shader/material.fs.glsl");

  data->model = cm_model_load("res/models/monkey.obj");

  mat4s transforms = glms_translate_make((vec3s){0});
  cm_mesh_attach_transforms(&data->model, &transforms, 1);

  camera_register_callbacks(&scene->camera);

  glfwSwapInterval(0);
  scene->state = data;
  return true;
}

static void models_free(CmScene *scene) {
  SceneData *data = scene->state;
  cm_mesh_delete(&data->model);
  cm_shader_delete(&data->shader);
  free(scene->state);
}

static void models_update(CmScene *scene, float dt) {
  (void)dt;
  SceneData *data = scene->state;

  cm_shader_bind(&data->shader);
  cm_shader_set_mat4(&data->shader, "u_vp", scene->camera.vp);
  cm_shader_set_vec3(&data->shader, "u_view_pos", scene->camera.position);
  cm_shader_set_vec3(&data->shader, "u_light_pos", scene->camera.position);
  cm_shader_set_vec4(&data->shader, "u_light_color", (vec4s){{1, 1, 1, 1}});
  const Material material = {
      .ambient = {{0.0215, 0.1745, 0.0215}},
      .diffuse = {{0.07568, 0.61424, 0.07568}},
      .specular = {{0.633, 0.727811, 0.633}},
      .shininess = 0.6,
  };
  material_set(&data->shader, &material);

  // RENDER HERE
  cm_mesh_draw(&data->model);
  cm_shader_unbind();
}

CmSceneInterface scene_models(void) {
  return (CmSceneInterface){
      .init = models_init,
      .free = models_free,
      .update = models_update,
      .layers =
          {
              {sandbox_fps, true},
              {0},
          },
  };
}
