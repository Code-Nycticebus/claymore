#include "claymore/entrypoint.h"

#include "claymore.h"

typedef struct {
  mat4 view;
  mat4 projection;
} Sandbox;

#define SANDBOX_MAX_QUADS 317

static float rand_f32(void) { return (float)rand() / (float)RAND_MAX; }

static void sandbox_init(CmScene *scene) {
  clib_log_info("sandbox init");
  Sandbox *sandbox = arena_alloc(&scene->arena, sizeof(Sandbox));

  cm_window_bg_color((vec3){0.15f, 0.15f, 0.15f});

  glm_ortho(0, 720, 0, 420, -1, 100.f, sandbox->view);
  glm_mat4_identity(sandbox->projection);

  scene->data = sandbox;
}

static void sandbox_update(CmScene *scene, double deltatime) {
  static double timer = 0;
  if ((timer += deltatime) >= 1) {
    clib_log_info("fps: %f", 1 / deltatime);
    timer = 0;
  }
  Sandbox *sandbox = scene->data;

  mat4 vp;
  glm_mat4_mul(sandbox->view, sandbox->projection, vp);

  cm_quad_begin(vp);
  const float size = 10;
  static float r = 0;
  r += 3.141f * (float)deltatime;
  for (usize i = 0; i < SANDBOX_MAX_QUADS; i++) {
    for (usize j = 0; j < SANDBOX_MAX_QUADS; j++) {
      cm_quad_push((vec2){(float)i * size, (float)j * size}, (vec2){size, size},
                   r, (vec4){rand_f32(), rand_f32(), rand_f32(), 1});
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
  (void)sandbox;

  cm_event_key(event, {
    clib_log_info("Key Event: ");
    if (key->code == CM_KEY_ESCAPE) {
      cm_window_close(true);
    }
  });

  cm_event_cursor(event, {
    clib_log_info("Cursor Event: " VEC2_FMT, VEC2_ARG(cursor->pos));
  });

  cm_event_mouse(event, {
    clib_log_info("Mouse Event: " VEC2_FMT, VEC2_ARG(mouse->pos));
  });

  cm_event_scroll(event, {
    clib_log_info("Scroll Event: " VEC2_FMT, VEC2_ARG(scroll->offset));
  });

  cm_event_resize(event, {
    clib_log_info("Resize Event " VEC2_FMT, VEC2_ARG(resize->size));
  });

  cm_event_drop(event, {
    clib_log_info("Drop event: ");
    for (usize i = 0; i < drop->count; i++) {
      clib_log_info("- %s", drop->files[i]);
    }
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
