#include "claymore/entrypoint.h"

#include "claymore.h"

#include "scenes/fps.h"

const usize height = 420;
const usize width = 720;
const float aspect = (float)width / (float)height;

const float speed = 1000.f;

typedef struct {
  float zoom;
  vec3 position;
  mat4 view;
  mat4 projection;
} Camera;

typedef struct {
  CmCamera2D camera;
} Benchmark;

#define MAX_QUADS 317

static void on_event(CmScene *scene, CmEvent *event) {
  Benchmark *benchmark = scene->data;

  cm_event_key(event, {
    if (key->code == RGFW_Escape) {
      cm_app_quit();
    }
  });

  cm_event_scroll(event, {
    const float min_zoom = 1.F;
    const float scroll_speed = 10.F;
    float zoom = benchmark->camera.zoom;
    zoom = glm_max(zoom - scroll->offset[1] * (zoom / scroll_speed), min_zoom);
    glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 100.F,
              benchmark->camera.base.projection);
    benchmark->camera.zoom = zoom;
    cm_camera_update(&benchmark->camera);
  });
}

static void init(CmScene *scene) {
  cebus_log_info("benchmark init");
  Benchmark *benchmark = cm_scene_alloc_data(scene, sizeof(Benchmark));

  const vec3 bg_color = {0.15f, 0.15f, 0.15f};
  cm_app_background(bg_color);

  const float zoom = 100.f;
  benchmark->camera.zoom = zoom;
  glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 100.F,
            benchmark->camera.base.projection);

  glm_vec3_zero(benchmark->camera.position);
  glm_mat4_identity(benchmark->camera.base.view);
  glm_translate(benchmark->camera.base.view, (vec3){0});
  cm_camera2D_screen(&benchmark->camera);

  cm_scene_push(scene, fps);
}

static void update(CmScene *scene, double deltatime) {
  static usize grid = 1;

  static double timer = 0;
  const float fps = 1 / deltatime;
  if ((timer += deltatime) >= 1) {
    cebus_log_info("grid: %" USIZE_FMT, grid * grid);
    static usize count = 0;
    count++;
    static usize max = 0;
    max += grid * grid;
    cebus_log_info("avg: %" USIZE_FMT, max / count);
    timer = 0;
  }
  Benchmark *benchmark = scene->data;

  const float fps_threshold = 60;
  grid += fps > fps_threshold ? 1 : grid > 0 ? -1 : 0;

  vec3 dir = {0};

  RGFW_window *window = cm_app_window();
  if (RGFW_isPressedI(window, RGFW_w)) {
    dir[1] = -speed;
  }
  if (RGFW_isPressedI(window, RGFW_s)) {
    dir[1] = speed;
  }
  if (RGFW_isPressedI(window, RGFW_a)) {
    dir[0] = speed;
  }
  if (RGFW_isPressedI(window, RGFW_d)) {
    dir[0] = -speed;
  }

  const float zoom = benchmark->camera.zoom / 100;
  glm_vec2_scale(dir, deltatime * zoom, dir);
  glm_vec3_add(benchmark->camera.position, dir, benchmark->camera.position);

  glm_mat4_identity(benchmark->camera.base.view);
  glm_translate(benchmark->camera.base.view, benchmark->camera.position);
  benchmark->camera.base.dirty = true;

  cm_renderer2D_begin(&benchmark->camera);
  {
    const float size = 100;
    static float r = 0;
    const vec4 quad_color = {.2f, .2f, .8f, 1.f};
    r += (F64_PI / 2) * deltatime;
    for (usize i = 0; i < grid; i++) {
      for (usize j = 0; j < grid; j++) {
        const vec2 pos = {i * size, j * size};
        cm_quad(pos, (vec2){size, size}, r, quad_color);
      }
    }
  }
  cm_renderer2D_end();
}

static CmSceneInterface *benchmark(void) {
  static CmSceneInterface interface = {
      .init = init,
      .update = update,
      .event = on_event,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = width, .height = height, .title = "benchmark"},
      .main = benchmark,
  };
  return &config;
}
