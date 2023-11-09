#include "light.h"
#include <stdlib.h>

#include "camera.h"

typedef struct {
  CmShader shader;
  CmMesh model;
} SceneData;

static float rand_float(void) { return (float)rand() / (float)RAND_MAX; }

bool light_init(CmScene *scene) {
  SceneData *data = malloc(sizeof(SceneData));
  data->shader = cm_shader_load_from_file("res/shader/cube.vs.glsl",
                                          "res/shader/cube.fs.glsl");

  data->model = cm_model_load("res/models/cube.obj");
  const uint32_t cube_count = 40000;
  mat4s *transforms = malloc(sizeof(mat4s) * cube_count);
  vec4s *colors = malloc(sizeof(vec4s) * cube_count);
  for (uint32_t i = 0; i < cube_count; i++) {
    const float spread = rand_float() * 1000 + 50;
    const vec3s pos = glms_vec3_scale(glms_vec3_normalize((vec3s){{
                                          (rand_float() - 0.5F),
                                          (rand_float() - 0.5F),
                                          (rand_float() - 0.5F),
                                      }}),
                                      spread);
    transforms[i] = glms_translate_make(pos);
    const vec4s color = {{
        rand_float(),
        rand_float(),
        rand_float(),
        1,
    }};
    colors[i] = color;
  }
  cm_mesh_attach_colors_instanced(&data->model, colors, cube_count);
  free(colors);
  cm_mesh_attach_transforms(&data->model, transforms, cube_count);
  free(transforms);

  const float aspect =
      (float)scene->app->window->width / (float)scene->app->window->height;
  const float fov = 90;
  scene->camera =
      cm_camera_init_perspective((vec3s){{0, 0, 4}}, (vec3s){0}, fov, aspect);
  camera_register_callbacks(&scene->camera);

  scene->state = data;
  glfwSwapInterval(0);
  return true;
}

void light_free(CmScene *scene) {
  SceneData *data = scene->state;
  cm_shader_delete(&data->shader);
  cm_mesh_delete(&data->model);
  free(data);
}

void light_update(CmScene *scene, float dt) {
  (void)dt;
  SceneData *data = scene->state;

  cm_shader_bind(&data->shader);
  static float rotation = 0;
  mat4s mvp =
      glms_rotate(scene->camera.vp, glm_rad(rotation), (vec3s){{0, 1, 1}});
  rotation += 3 * dt;
  cm_shader_set_mat4(&data->shader, "u_vp", mvp);
  cm_shader_set_vec3(&data->shader, "u_view_pos", scene->camera.position);
  cm_shader_set_vec3(&data->shader, "u_light_pos", (vec3s){0});
  cm_shader_set_vec4(&data->shader, "u_light_color", (vec4s){{1, 1, 1, 1}});

  cm_mesh_draw(&data->model);

  cm_shader_unbind();
  (void)data;
}

CmSceneInterface scene_light(void) {
  CmLayerInterface sandbox_fps(void);
  return (CmSceneInterface){
      .init = light_init,
      .update = light_update,
      .free = light_free,
      .layers =
          {
              {sandbox_fps, true},
              {0},
          },
  };
}
