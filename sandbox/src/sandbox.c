#include "cglm/vec3.h"
#include "claymore/entrypoint.h"

#include "claymore.h"

const usize height = 420;
const usize width = 720;
const float aspect = (float)width / (float)height;

typedef struct {
  float zoom;
  vec3 position;
  mat4 view;
  mat4 projection;
} Camera;

typedef struct {
  Camera camera;
} Sandbox;

#define SANDBOX_MAX_QUADS 317

static float rand_f32(void) { return (float)rand() / (float)RAND_MAX; }

static void sandbox_init(CmScene *scene) {
  clib_log_info("sandbox init");
  Sandbox *sandbox = arena_alloc(&scene->arena, sizeof(Sandbox));

  const vec3 bg_color = {0.15f, 0.15f, 0.15f};
  cm_window_bg_color(bg_color);

  const float zoom = 100.f;
  sandbox->camera.zoom = zoom;
  glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 100.F,
            sandbox->camera.projection);

  glm_vec3_zero(sandbox->camera.position);
  glm_mat4_identity(sandbox->camera.view);
  glm_translate(sandbox->camera.view, (vec3){0});

  scene->data = sandbox;
}

static void sandbox_update(CmScene *scene, double deltatime) {
  static usize grid = 1;

  static double timer = 0;
  if ((timer += deltatime) >= 1) {
    clib_log_info("fps: %.0f", 1 / deltatime);
    clib_log_info("grid: %" USIZE_FMT, grid * grid);
    timer = 0;
  }
  Sandbox *sandbox = scene->data;

  if (1 / deltatime > 30) {
    grid++;
  } else {
    grid -= grid > 0 ? 1 : 0;
  }

  vec3 dir = {0};
  if (cm_event_key_pressed(CM_KEY_W)) {
    glm_vec3_copy((vec3){0, -1000, 0}, dir);
  } else if (cm_event_key_pressed(CM_KEY_S)) {
    glm_vec3_copy((vec3){0, 1000, 0}, dir);
  } else if (cm_event_key_pressed(CM_KEY_A)) {
    glm_vec3_copy((vec3){1000, 0, 0}, dir);
  } else if (cm_event_key_pressed(CM_KEY_D)) {
    glm_vec3_copy((vec3){-1000, 0, 0}, dir);
  }

  glm_vec3_scale(dir, deltatime * sandbox->camera.zoom / 100, dir);
  glm_vec3_add(sandbox->camera.position, dir, sandbox->camera.position);

  glm_mat4_identity(sandbox->camera.view);
  glm_translate(sandbox->camera.view, sandbox->camera.position);

  mat4 vp;
  glm_mat4_mul(sandbox->camera.projection, sandbox->camera.view, vp);
  cm_quad_begin(vp);
  const float size = 100;
  static float r = 0;
  r += (PI / 2) * deltatime;
  for (usize i = 0; i < grid; i++) {
    for (usize j = 0; j < grid; j++) {
      cm_quad_push((vec2){i * size, j * size}, (vec2){size, size}, r,
                   (vec4){rand_f32(), rand_f32(), rand_f32(), 1});
    }
  }
  cm_quad_end();
}

static void sandbox_free(CmScene *scene) {
  (void)scene;
  clib_log_info("sandbox free");
}

static void sandbox_on_event(CmScene *scene, CmEvent *event) {
  Sandbox *sandbox = scene->data;

  cm_event_key(event, {
    clib_log_info("Key Event: ");
    if (key->code == CM_KEY_ESCAPE) {
      cm_window_close(true);
    }
  });

  cm_event_scroll(event, {
    clib_log_info("Scroll Event: " VEC2_FMT, VEC2_ARG(scroll->offset));
    const float min_zoom = 1.F;
    const float scroll_speed = 10.F;
    float zoom = sandbox->camera.zoom;
    zoom = glm_max(zoom - scroll->offset[1] * (zoom / scroll_speed), min_zoom);
    glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 100.F,
              sandbox->camera.projection);
    sandbox->camera.zoom = zoom;
  });
}

static CmSceneInterface *sandbox_scene_init(void) {
  static CmSceneInterface sandbox = {
      .init = sandbox_init,
      .update = sandbox_update,
      .free = sandbox_free,
      .on_event = sandbox_on_event,
  };
  return &sandbox;
}

const ClaymoreConfig *claymore_init(void) {
  static const ClaymoreConfig config = {
      .window =
          {
              .width = 720,
              .height = 420,
              .title = "sandbox",
          },
      .scene = sandbox_scene_init,
  };
  return &config;
}
