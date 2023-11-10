#include "noise/noise.h"
#include <stdlib.h>

#define TEXT_HEIGHT 500
#define TEXT_WIDTH 500

#define GRID_SIZE 50

typedef struct {
  CmTexture texture;
  CmShader shader;
  size_t octave;
  float scale;
  float *sample_map;
} SceneData;

void noise_gen(size_t width, size_t height, size_t octave_count,
               float scaling_bias, float *sample_map, uint8_t *output) {
  for (size_t x = 0; x < width; x++) {
    for (size_t y = 0; y < height; y++) {
      float noise = 0.0F;
      float scale_acc = 0.0F;
      float scale = 1.0F;

      for (size_t o = 0; o < octave_count; o++) {
        size_t pitch = width >> o;
        ivec2s sample1 = {{
            (x / pitch) * pitch,
            (y / pitch) * pitch,
        }};

        ivec2s sample2 = {{
            (sample1.x + pitch) % width,
            (sample1.y + pitch) % height,
        }};

        vec2s blend = {{
            (float)(x - sample1.x) / (float)pitch,
            (float)(y - sample1.y) / (float)pitch,
        }};

        vec2s sample_value = {{
            glm_lerp(sample_map[sample1.y * width + sample1.x],
                     sample_map[sample1.y * width + sample2.x], blend.x),
            glm_lerp(sample_map[sample2.y * width + sample1.x],
                     sample_map[sample2.y * width + sample2.x], blend.x),
        }};
        noise += glm_lerp(sample_value.x, sample_value.y, blend.y) * scale;
        scale_acc += scale;
        scale /= scaling_bias;
      }
      float value = (noise / scale_acc);
      output[y * width + x] = value * UINT8_MAX;
    }
  }
}

static void sample_map_update(uint32_t count, float output[static count]) {
  for (size_t i = 0; i < count; i++) {
    output[i] = (float)rand() / (float)RAND_MAX;
  }
}

static void noise_map_update(CmTexture *texture, float *sample_map, float scale,
                             size_t octave) {
  uint8_t *tex = calloc(texture->width * texture->height, sizeof(*tex));

  noise_gen(texture->width, texture->height, octave, scale, sample_map, tex);
  cm_texture_update(texture, tex);

  free(tex);
}

static void key_event(CmKeyEvent *event, SceneData *data) {
  if (event->action == CM_KEY_PRESS || event->action == CM_KEY_REPEAT) {
    if (event->code == CM_KEY_F5) {
      sample_map_update(data->texture.height * data->texture.width,
                        data->sample_map);
      noise_map_update(&data->texture, data->sample_map, data->scale,
                       data->octave);
    } else if (event->code == CM_KEY_SPACE) {
      const size_t max_octaves = 8;
      data->octave = data->octave % max_octaves + 1;
      noise_map_update(&data->texture, data->sample_map, data->scale,
                       data->octave);
    } else if (event->code == CM_KEY_UP || event->code == CM_KEY_DOWN) {
      const float offset = .2F;
      const float min = .2F;
      const float max = 1.5F;
      data->scale += event->code == CM_KEY_UP ? offset : -offset;
      data->scale = glm_clamp(data->scale, min, max);
      noise_map_update(&data->texture, data->sample_map, data->scale,
                       data->octave);
    }
  }
}

static bool noise_init(CmScene *scene) {
  cm_window_set_size(scene->app->window, TEXT_WIDTH, TEXT_HEIGHT);
  SceneData *data = malloc(sizeof(SceneData));

  data->sample_map = malloc(TEXT_HEIGHT * TEXT_WIDTH * sizeof(float));
  sample_map_update(TEXT_WIDTH * TEXT_HEIGHT, data->sample_map);

  data->texture =
      cm_texture_create_from_memory(TEXT_WIDTH, TEXT_HEIGHT, NULL, CM_TEX_R);
  data->shader = cm_shader_load_from_file("res/shader/noise.vs.glsl",
                                          "res/shader/noise.fs.glsl");

  scene->camera = cm_camera_init_screen((vec3s){0}, scene->app->window->width,
                                        scene->app->window->height);

  data->scale = 1.F;
  data->octave = 1;
  noise_map_update(&data->texture, data->sample_map, data->scale, data->octave);

  cm_event_subscribe(CM_EVENT_KEYBOARD, (cm_event_callback)key_event, data);

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
  free(data->sample_map);
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
