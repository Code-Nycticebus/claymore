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

  data->shader = cm_shader_load_from_file("res/shader/cube.vs.glsl",
                                          "res/shader/cube.fs.glsl");

  data->model = cm_model_load("res/models/cube.obj");

  const size_t transform_count = 30;
  const float margin = 30.F;
  mat4s *transforms = malloc(sizeof(mat4s) * transform_count * transform_count *
                             transform_count);
  size_t index = 0;
  for (size_t x = 0; x < transform_count; x++) {
    for (size_t y = 0; y < transform_count; y++) {
      for (size_t z = 0; z < transform_count; z++) {
        transforms[index++] =
            glms_translate_make((vec3s){{x * margin, y * margin, z * margin}});
      }
    }
  }

  cm_mesh_attach_transforms(&data->model, transforms,
                            transform_count * transform_count *
                                transform_count);

  camera_register_callbacks(&scene->camera);

  glfwSwapInterval(0);
  scene->state = data;
  return true;
}

static void models_free(CmScene *scene) { free(scene->state); }

static void models_update(CmScene *scene, float dt) {
  (void)dt;
  SceneData *data = scene->state;

  cm_shader_bind(&data->shader);
  cm_shader_set_mat4(&data->shader, "u_vp", scene->camera.vp);
  cm_shader_set_vec3(&data->shader, "u_view_pos", scene->camera.position);
  cm_shader_set_vec3(&data->shader, "u_light_pos", scene->camera.position);
  cm_shader_set_vec4(&data->shader, "u_light_color", (vec4s){{1, 1, 1, 1}});
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
              sandbox_fps,
              NULL,
          },
  };
}
