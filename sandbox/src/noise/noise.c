#include "noise/noise.h"
#include <stdlib.h>

typedef struct {
  CmTexture texture;
  CmShader shader;
} SceneData;

static bool noise_init(CmScene *scene) {
  cm_window_set_size(scene->app->window, 500, 500);
  SceneData *data = malloc(sizeof(SceneData));
  uint8_t tex[500 * 500] = {0};
  for (size_t i = 0; i < 500 * 500; i++) {
    tex[i] = rand() % 255;
  }
  data->texture = cm_texture_create_from_memory(500, 500, tex, CM_TEX_R);
  data->shader = cm_shader_load_from_file("res/shader/noise.vs.glsl",
                                          "res/shader/noise.fs.glsl");

  scene->camera = cm_camera_init_screen((vec3s){0}, scene->app->window->width,
                                        scene->app->window->height);

  scene->state = data;
  return true;
}

static void noise_update(CmScene *scene, float dt) {
  (void)dt;
  SceneData *data = scene->state;
  cm_shader_bind(&data->shader);
  cm_shader_set_mat4(&data->shader, "u_vp", scene->camera.vp);
  cm_shader_set_i32(&data->shader, "u_texture", 0);

  cm_texture_bind(&data->texture, 0);

  cm_renderer2d_begin();
  cm_renderer2d_push_quad_textured(
      (vec2s){{0, 0}}, 0,
      (vec2s){{scene->app->window->width, scene->app->window->height}},
      (vec2s){{0, 0}}, (vec2s){{1, 1}});
  cm_renderer2d_end();

  cm_shader_unbind();
}

static void noise_free(CmScene *scene) {
  SceneData *data = scene->state;
  cm_shader_delete(&data->shader);
  cm_texture_delete(&data->texture);
  free(scene->state);
}

CmSceneInterface scene_noise(void) {
  return (CmSceneInterface){
      .init = noise_init,
      .free = noise_free,
      .update = noise_update,
      .layers = {0},
  };
}
