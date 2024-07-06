#include "claymore.h"

#define HEIGTH 420
#define WIDTH 720
const float aspect = (float)WIDTH / (float)HEIGTH;

const float speed = 1000.f;

typedef struct {
  CmCamera2D camera;
  CmCamera2D overlay;
  CmFont *font;
  usize grid;
} Benchmark;

static void on_event(CmScene *scene, CmEvent *event) {
  Benchmark *benchmark = cm_scene_data(scene);

  cm_event_scroll(event, {
    const float scroll_speed = 10.F;
    float zoom = benchmark->camera.zoom;
    zoom = zoom - scroll->offset[1] * (zoom / scroll_speed);
    cm_camera2D_set_zoom(&benchmark->camera, zoom);
  });

  cm_event_resize(event, {
    cm_camera2D_screen(&benchmark->overlay);
    cm_camera2D_set_aspect(&benchmark->camera, cm_app_aspect());
  });
}

static void init(CmScene *scene) {
  Benchmark *benchmark = cm_scene_data(scene);

  cm_camera2D_ortho(&benchmark->camera, (vec2){0}, aspect, 100.f);

  cm_camera2D_screen(&benchmark->overlay);

  const Str font = STR("assets/fonts/Ubuntu.ttf");
  const float height = 32.f;
  benchmark->font = cm_font_from_file(&scene->gpu, font, height, ErrPanic);
}

static void frame_update(CmScene *scene) {
  Benchmark *benchmark = cm_scene_data(scene);

  const float fps_threshold = 60;
  double deltatime = cm_app_deltatime();
  const float fps = 1 / deltatime;
  benchmark->grid += fps > fps_threshold ? 1 : benchmark->grid > 0 ? -1 : 0;

  vec2 dir = {0};

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
  glm_vec2_scale(dir, zoom, dir);
  glm_vec2_scale(dir, deltatime, dir);
  cm_camera2D_move(&benchmark->camera, dir);

  cm_2D_begin(&benchmark->camera);
  {
    const float size = 10;
    static float r = 0;
    const vec4 quad_color = {.2f, .2f, .8f, 1.f};
    r += (F64_PI / 2) * deltatime;
    for (usize i = 0; i < benchmark->grid; i++) {
      for (usize j = 0; j < benchmark->grid; j++) {
        const vec2 pos = {i * size, j * size};
        cm_2D_quad(pos, (vec2){size, size}, r, quad_color);
      }
    }
  }
  cm_2D_end();

  cm_2D_begin(&benchmark->overlay);
  {
    char buffer[20];
    usize s =
        snprintf(buffer, 20, "%8" U64_FMT, benchmark->grid * benchmark->grid);
    cm_2D_text(benchmark->font, (vec2){window->r.w - 16 * 9, 0},
               str_from_parts(s, buffer));
  }
  cm_2D_end();
}

CmSceneInterface *benchmark(void) {
  static CmSceneInterface interface = {
      CM_SCENE(Benchmark),
      .init = init,
      .frame_update = frame_update,
      .event = on_event,
  };
  return &interface;
}
