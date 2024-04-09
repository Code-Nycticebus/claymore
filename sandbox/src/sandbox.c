#include "claymore/entrypoint.h"

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

  vec2 window_size;
  cm_window_get_size(window_size);
  glm_ortho(0, window_size[0], window_size[1], 0, -1.F, 100.F,
            sandbox->camera.projection);

  glm_vec3_zero(sandbox->camera.position);
  glm_mat4_identity(sandbox->camera.view);
  glm_translate(sandbox->camera.view, (vec3){0});

  const float font_size = 32.f;
  sandbox->font = cm_font_init(&scene->gpu, STR("res/fonts/Ubuntu.ttf"),
                               font_size, ErrPanic);

  sandbox->texture =
      cm_texture_from_file(STR("res/textures/claymore-sword.png"), ErrPanic);

  cm_scene_push(scene, fps_scene_init);
}

static void update(CmScene *scene, double dt) {
  (void)dt;
  Sandbox *sandbox = scene->data;
  mat4 vp;
  glm_mat4_mul(sandbox->camera.projection, sandbox->camera.view, vp);

  {
    cm_sprite_begin(vp, &sandbox->texture);
    const vec2 size = {100.f, 100.f};
    const f32 margin = 10.f;
    for (usize i = 0; i < 4; ++i) {
      float y = (size[1] + margin) * i;
      for (usize j = 0; j < 4; ++j) {
        float x = (size[0] + margin) * j;
        cm_sprite_push((vec2){x, y}, size, 0, (vec2){0}, (vec2){1, 1});
      }
    }
    cm_sprite_end();
  }
  {
    Str msg = STR("This is Claymore!");
    vec2 pos;
    cm_event_cursor_position(pos);
    const float margin = 5;
    const float font_size = 32;
    const float char_width = 13;
    cm_quad_begin(vp);
    cm_quad_push((vec2){pos[0], pos[1]},
                 (vec2){msg.len * char_width, font_size + margin}, 0,
                 (vec4){1, 0, 0, 1});
    cm_quad_end();
    cm_font_draw(sandbox->font, vp, pos, msg);
  }
}

static CmSceneInterface *scene_init(void) {
  static CmSceneInterface sandbox = {
      .init = init,
      .update = update,
      .event = event,
  };
  return &sandbox;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 720, .height = 420, .title = "sandbox"},
      .main = scene_init,
  };
  return &config;
}
