#include "light.h"
#include <stdlib.h>

#include "camera.h"

typedef struct {
  struct {
    CmShader shader;
    CmMesh model;
  } cube;
  struct {
    CmShader shader;
    CmMesh mesh;
    vec3s pos;
    vec4s color;
  } light;
} SceneData;

static float rand_float(void) { return (float)rand() / (float)RAND_MAX; }

bool light_init(CmScene *scene) {
  SceneData *data = malloc(sizeof(SceneData));
  data->cube.shader = cm_shader_load_from_file("res/shader/cube.vs.glsl",
                                               "res/shader/cube.fs.glsl");

  const float min_spread = 50.F;
  const float max_spread = 500.F;

  data->cube.model = cm_model_load("res/models/cube.obj");
  const uint32_t cube_count = 40000;
  mat4s *transforms = malloc(sizeof(mat4s) * cube_count);
  vec4s *colors = malloc(sizeof(vec4s) * cube_count);
  for (uint32_t i = 0; i < cube_count; i++) {
    const float spread = rand_float() * max_spread + min_spread;
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
  cm_mesh_attach_colors_instanced(&data->cube.model, colors, cube_count);
  free(colors);
  cm_mesh_attach_transforms(&data->cube.model, transforms, cube_count);
  free(transforms);

  const float aspect =
      (float)scene->app->window->width / (float)scene->app->window->height;
  const float fov = 90;
  scene->camera = cm_camera_perspective_init((vec3s){{0, 0, min_spread * 2}},
                                             (vec3s){0}, fov, aspect);
  camera_register_callbacks(&scene->camera);

  data->light.mesh = cm_model_load("res/models/sphere.obj");
  data->light.shader = cm_shader_load_from_file(
      "res/shader/basic_instanced.vs.glsl", "res/shader/basic.fs.glsl");
  data->light.pos = (vec3s){0};
  const vec4s l_color = (vec4s){{1, 1, 0.85, 1}};
  data->light.color = l_color;
  cm_mesh_attach_colors_instanced(&data->light.mesh, &data->light.color, 1);
  mat4s translation = glms_translate_make(data->light.pos);
  mat4s scale = glms_scale_make(
      (vec3s){{min_spread / 2, min_spread / 2, min_spread / 2}});
  mat4s transform = glms_mul(translation, scale);
  cm_mesh_attach_transforms(&data->light.mesh, &transform, 1);

  scene->state = data;
  glfwSwapInterval(0);
  return true;
}

void light_free(CmScene *scene) {
  SceneData *data = scene->state;
  cm_shader_delete(&data->cube.shader);
  cm_mesh_delete(&data->cube.model);
  free(data);
}

void light_update(CmScene *scene, float dt) {
  SceneData *data = scene->state;

  static float rotation = 0;
  mat4s mvp =
      glms_rotate(scene->camera.vp, glm_rad(rotation), (vec3s){{0, 1, 1}});
  rotation += 3 * dt;

  cm_shader_bind(&data->light.shader);
  cm_shader_set_mat4(&data->light.shader, "u_vp", mvp);
  cm_mesh_draw(&data->light.mesh);
  cm_shader_unbind();

  cm_shader_bind(&data->cube.shader);
  cm_shader_set_mat4(&data->cube.shader, "u_vp", mvp);
  cm_shader_set_vec3(&data->cube.shader, "u_view_pos", scene->camera.position);
  cm_shader_set_vec3(&data->cube.shader, "u_light_pos", (vec3s){0});
  cm_shader_set_vec4(&data->cube.shader, "u_light_color",
                     (vec4s){{1, 1, 1, 1}});

  cm_mesh_draw(&data->cube.model);

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
