#include "claymore/entrypoint.h"

#include "claymore.h"

const usize height = 420;
const usize width = 720;
const float aspect = (float)width / (float)height;

const float speed = 1000.f;

CmSceneInterface *fps_scene_init(void);

typedef struct {
  float zoom;
  vec3 position;
  mat4 view;
  mat4 projection;
} Camera;

typedef struct {
  Camera camera;
} Benchmark;

#define MAX_QUADS 317

static void on_event(CmScene *scene, CmEvent *event) {
  Benchmark *benchmark = scene->data;

  cm_event_key(event, {
    if (key->code == CM_KEY_ESCAPE) {
      cm_window_close(true);
    }
  });

  cm_event_scroll(event, {
    const float min_zoom = 1.F;
    const float scroll_speed = 10.F;
    float zoom = benchmark->camera.zoom;
    zoom = glm_max(zoom - scroll->offset[1] * (zoom / scroll_speed), min_zoom);
    glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 100.F,
              benchmark->camera.projection);
    benchmark->camera.zoom = zoom;
  });
}

static void init(CmScene *scene) {
  clib_log_info("benchmark init");
  Benchmark *benchmark = cm_scene_alloc_data(scene, sizeof(Benchmark));

  const vec3 bg_color = {0.15f, 0.15f, 0.15f};
  cm_window_set_bg(bg_color);

  const float zoom = 100.f;
  benchmark->camera.zoom = zoom;
  glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 100.F,
            benchmark->camera.projection);

  glm_vec3_zero(benchmark->camera.position);
  glm_mat4_identity(benchmark->camera.view);
  glm_translate(benchmark->camera.view, (vec3){0});

  cm_scene_push(scene, fps_scene_init);
}

static void update(CmScene *scene, double deltatime) {
  static usize grid = 1;

  static double timer = 0;
  const float fps = 1 / deltatime;
  if ((timer += deltatime) >= 1) {
    clib_log_info("grid: %" USIZE_FMT, grid * grid);
    static usize count = 0;
    count++;
    static usize max = 0;
    max += grid * grid;
    clib_log_info("avg: %" USIZE_FMT, max / count);
    timer = 0;
  }
  Benchmark *benchmark = scene->data;

  const float fps_threshold = 60;
  grid += fps > fps_threshold ? 1 : grid > 0 ? -1 : 0;

  vec3 dir = {0};
  if (cm_event_key_pressed(CM_KEY_W)) {
    dir[1] = -speed;
  }
  if (cm_event_key_pressed(CM_KEY_S)) {
    dir[1] = speed;
  }
  if (cm_event_key_pressed(CM_KEY_A)) {
    dir[0] = speed;
  }
  if (cm_event_key_pressed(CM_KEY_D)) {
    dir[0] = -speed;
  }

  const float zoom = benchmark->camera.zoom / 100;
  glm_vec3_scale(dir, deltatime * zoom, dir);
  glm_vec3_add(benchmark->camera.position, dir, benchmark->camera.position);

  glm_mat4_identity(benchmark->camera.view);
  glm_translate(benchmark->camera.view, benchmark->camera.position);

  mat4 vp;
  glm_mat4_mul(benchmark->camera.projection, benchmark->camera.view, vp);
  cm_quad_begin(vp);
  const float size = 100;
  static float r = 0;
  const vec4 quad_color = {.2f, .2f, .8f, 1.f};
  r += (F64_PI / 2) * deltatime;
  for (usize i = 0; i < grid; i++) {
    for (usize j = 0; j < grid; j++) {
      const vec2 pos = {i * size, j * size};
      cm_quad_push(pos, (vec2){size, size}, r, quad_color);
    }
  }
  cm_quad_end();
}

static CmSceneInterface *scene_init(void) {
  static CmSceneInterface benchmark = {
      .init = init,
      .update = update,
      .event = on_event,
  };
  return &benchmark;
}

const ClaymoreConfig *claymore_init(void) {
  static const ClaymoreConfig config = {
      .window = {.width = width, .height = height, .title = "benchmark"},
      .main = scene_init,
  };
  return &config;
}
