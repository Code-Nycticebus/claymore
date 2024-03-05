#include "claymore/entrypoint.h"

#include "claymore/renderer/font.h"

CmSceneInterface *fps_scene_init(void);

typedef struct {
  float zoom;
  vec3 position;
  mat4 view;
  mat4 projection;
} Camera;

typedef struct {
  Camera camera;
  CmFont *font;
  CmTexture2D texture;
} Sandbox;

static void event(CmScene *scene, CmEvent *event) {
  Sandbox *sandbox = scene->data;
  (void)sandbox, (void)event;
  cm_event_key(event, {
    if (key->action == CM_KEY_PRESS && key->code == CM_KEY_ESCAPE) {
      cm_window_close(true);
    }
  });

  cm_event_cursor(event, { clib_log_info(VEC2_FMT, VEC2_ARG(cursor->pos)); });
}

static void init(CmScene *scene) {
  Sandbox *sandbox = cm_scene_alloc_data(scene, sizeof(Sandbox));

  glm_ortho(0, 720, 420, 0, -1.F, 100.F, sandbox->camera.projection);

  glm_vec3_zero(sandbox->camera.position);
  glm_mat4_identity(sandbox->camera.view);
  glm_translate(sandbox->camera.view, (vec3){0});

  sandbox->font = cm_font_init(&scene->gpu, STR("sandbox/res/fonts/Ubuntu.ttf"),
                               32.f, ErrPanic);

  cm_scene_push(scene, fps_scene_init);
}

static void update(CmScene *scene, double dt) {
  (void)dt;
  Sandbox *sandbox = scene->data;
  mat4 vp;
  glm_mat4_mul(sandbox->camera.projection, sandbox->camera.view, vp);

  cm_quad_begin(vp);
  cm_quad_push((vec2){10, 50}, (vec2){100, 150}, 0, (vec4){1, 0, 0, 1});
  cm_quad_end();
  cm_font_draw(sandbox->font, vp, (vec3){0, 50, 0}, STR("Hello World!"));
}

static CmSceneInterface *scene_init(void) {
  static CmSceneInterface sandbox = {
      .init = init,
      .update = update,
      .event = event,
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
      .main = scene_init,
  };
  return &config;
}
