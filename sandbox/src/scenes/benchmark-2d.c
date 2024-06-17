#include "claymore.h"

#include "utils/fps.h"

#define HEIGTH 420
#define WIDTH 720
const float aspect = (float)WIDTH / (float)HEIGTH;

const float speed = 1000.f;

typedef struct {
  float zoom;
  vec3 position;
  mat4 view;
  mat4 projection;
} Camera;

typedef struct {
  CmCamera2D camera;
  CmCamera2D overlay;
  CmFont *font;
} Benchmark;

#define MAX_QUADS 317

static void on_event(CmScene *scene, CmEvent *event) {
  Benchmark *benchmark = scene->data;

  cm_event_scroll(event, {
    const float min_zoom = 1.F;
    const float scroll_speed = 10.F;
    float zoom = benchmark->camera.zoom;
    zoom = glm_max(zoom - scroll->offset[1] * (zoom / scroll_speed), min_zoom);
    glm_ortho(-aspect * zoom, aspect * zoom, zoom, -zoom, .1f, 100.f,
              benchmark->camera.base.projection);
    benchmark->camera.zoom = zoom;
    cm_camera_update(&benchmark->camera);
  });
}

static void init(CmScene *scene) {
  Benchmark *benchmark = cm_scene_set_data(scene, sizeof(Benchmark));

  const vec2 position = {10, 0};
  const Str font = STR("assets/fonts/Ubuntu.ttf");
  const float height = 32.f;
  fps(scene, position, font, height);

  benchmark->camera.zoom = 100;
  cm_camera2D_ortho(&benchmark->camera, (vec2){0}, aspect, 100);

  cm_camera2D_screen(&benchmark->overlay);

  benchmark->font = cm_font_from_file(&scene->gpu, font, height, ErrPanic);
}

static void frame_update(CmScene *scene) {
  const float fps_threshold = 60;

  double deltatime = cm_app_deltatime();
  const float fps = 1 / deltatime;
  static usize grid = 1;
  grid += fps > fps_threshold ? 1 : grid > 0 ? -1 : 0;

  Benchmark *benchmark = scene->data;

  vec3 dir = {0};

  RGFW_window *window = cm_app_window();
  if (RGFW_isPressedI(window, RGFW_w)) {
    dir[1] = speed;
  }
  if (RGFW_isPressedI(window, RGFW_s)) {
    dir[1] = -speed;
  }
  if (RGFW_isPressedI(window, RGFW_a)) {
    dir[0] = speed;
  }
  if (RGFW_isPressedI(window, RGFW_d)) {
    dir[0] = -speed;
  }

  const float zoom = benchmark->camera.zoom / 100;
  glm_vec2_scale(dir, deltatime * zoom, dir);
  glm_vec3_add(benchmark->camera.base.position, dir,
               benchmark->camera.base.position);

  glm_mat4_identity(benchmark->camera.base.view);
  glm_translate(benchmark->camera.base.view, benchmark->camera.base.position);
  benchmark->camera.base.dirty = true;

  cm_2D_begin(&benchmark->camera);
  {
    const float size = 10;
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
  cm_2D_end();

  cm_2D_begin(&benchmark->overlay);
  {
    char buffer[20];
    usize s = snprintf(buffer, 20, "%" U64_FMT, grid * grid);
    cm_font(benchmark->font, (vec2){10, 32}, str_from_parts(s, buffer));
  }
  cm_2D_end();
}

CmSceneInterface *benchmark(void) {
  static CmSceneInterface interface = {
      .init = init,
      .frame_update = frame_update,
      .event = on_event,
  };
  return &interface;
}
