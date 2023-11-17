#include "noise/noise.h"
#include "cglm/struct/vec2.h"
#include <stdlib.h>

#define TEXT_HEIGHT 500
#define TEXT_WIDTH 500

#define GRID_SIZE 100

typedef struct {
  CmTexture texture;
  CmShader shader;
  size_t octave;
  size_t grid_size;
} SceneData;

vec2s randomGradient(int ix, int iy) {
  // No precomputed gradients mean this works for any number of grid coordinates
  const uint32_t w = 8 * sizeof(uint32_t);
  const uint32_t s = w / 2;
  uint32_t a = ix;
  uint32_t b = iy;
  a *= 3284157443;

  b ^= a << s | a >> (w - s);
  b *= 1911520717;

  a ^= b << s | b >> (w - s);
  a *= 2048419325;
  float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]

  // Create the vector from the angle
  vec2s v;
  v.x = sinf(random);
  v.y = cosf(random);

  return v;
}

// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int ix, int iy, float x, float y) {
  // Get gradient from integer coordinates
  vec2s gradient = randomGradient(ix, iy);

  // Compute the distance vector
  float dx = x - (float)ix;
  float dy = y - (float)iy;

  // Compute the dot-product
  return (dx * gradient.x + dy * gradient.y);
}

float interpolate(float a0, float a1, float w) {
  return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}

// Sample Perlin noise at coordinates x, y
float perlin(float x, float y) {
  // Determine grid cell corner coordinates
  int x0 = (int)x;
  int y0 = (int)y;
  int x1 = x0 + 1;
  int y1 = y0 + 1;

  // Compute Interpolation weights
  float sx = x - (float)x0;
  float sy = y - (float)y0;

  // Compute and interpolate top two corners
  float n0 = dotGridGradient(x0, y0, x, y);
  float n1 = dotGridGradient(x1, y0, x, y);
  float ix0 = interpolate(n0, n1, sx);

  // Compute and interpolate bottom two corners
  n0 = dotGridGradient(x0, y1, x, y);
  n1 = dotGridGradient(x1, y1, x, y);
  float ix1 = interpolate(n0, n1, sx);

  // Final step: interpolate between the two previously interpolated values, now
  // in y
  float value = interpolate(ix0, ix1, sy);
  return value;
}

void gradient_perlin_noise(uint32_t width, uint32_t height,
                           uint8_t output[static width * height],
                           size_t octaves, size_t grid_size) {
  printf("Generating\n");
  for (size_t x = 0; x < width; x++) {
    for (size_t y = 0; y < height; y++) {
      float val = 0;

      float freq = 1;
      float amp = 1;

      for (size_t i = 0; i < octaves; i++) {
        val += perlin(x * freq / grid_size, y * freq / grid_size) * amp;

        freq *= 2;
        amp /= 2;
      }

      // Contrast
      val *= 1.2F;

      // Clipping
      if (val > 1.0F) {
        val = 1.0F;
      } else if (val < -1.0F) {
        val = -1.0F;
      }

      // Convert 1 to -1 into 255 to 0
      output[x + y * width] = (uint8_t)(((val + 1.0F) * 0.5F) * UINT8_MAX);
    }
  }
  printf("done\n");
}

static void noise_map_update(CmTexture *texture, size_t grid_size,
                             size_t octave) {
  uint8_t *tex = calloc(texture->width * texture->height, sizeof(*tex));

  gradient_perlin_noise(texture->width, texture->height, tex, octave,
                        grid_size);
  cm_texture_update(texture, tex);

  free(tex);
}

static void key_event(CmKeyEvent *event, SceneData *data) {
  if (event->action == CM_KEY_PRESS || event->action == CM_KEY_REPEAT) {
    if (event->code == CM_KEY_UP || event->code == CM_KEY_DOWN) {
      data->grid_size += event->code == CM_KEY_UP ? 1 : -1;
      if (data->grid_size < 1) {
        data->grid_size = 1;
      }
      noise_map_update(&data->texture, data->grid_size, data->octave);
    } else if (CM_KEY_1 <= event->code && event->code <= CM_KEY_9) {
      data->octave = (event->code - CM_KEY_1) + 1;
      noise_map_update(&data->texture, data->grid_size, data->octave);
    }
  }
}

static bool noise_init(CmScene *scene) {
  cm_window_set_size(scene->app->window, TEXT_WIDTH, TEXT_HEIGHT);
  SceneData *data = malloc(sizeof(SceneData));

  data->texture =
      cm_texture_create_from_memory(TEXT_WIDTH, TEXT_HEIGHT, NULL, CM_TEX_R);
  data->shader = cm_shader_load_from_file("res/shader/noise.vs.glsl",
                                          "res/shader/noise.fs.glsl");

  scene->camera = cm_camera_screen_init((vec3s){0}, scene->app->window->width,
                                        scene->app->window->height);

  data->grid_size = 1;
  data->octave = 1;
  noise_map_update(&data->texture, data->grid_size, data->octave);

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
