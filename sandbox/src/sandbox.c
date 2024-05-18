#include "claymore/entrypoint.h"

#include "scenes/fps.h"

typedef struct {
  CmCamera2D camera;
  CmFont *font;
  CmTexture2D texture[2];
  vec2 mouse_pos;
} Sandbox;

static void event(CmScene *scene, CmEvent *event) {
  Sandbox *sandbox = scene->data;
  (void)sandbox, (void)event;
  cm_event_key(event, {
    if (key->action == CM_KEY_PRESS && key->code == CM_KEY_ESCAPE) {
      cm_window_close();
    }
  });
  cm_event_cursor(event, {
    sandbox->mouse_pos[0] = cursor->pos[0];
    sandbox->mouse_pos[1] = cursor->pos[1];
  });
}

static void init(CmScene *scene) {
  Sandbox *sandbox = cm_scene_alloc_data(scene, sizeof(Sandbox));

  cm_camera2d_screen(&sandbox->camera);

  const float font_size = 32.f;
  sandbox->font = cm_font_init(&scene->gpu, STR("res/fonts/Ubuntu.ttf"),
                               font_size, ErrPanic);

  sandbox->texture[0] = cm_texture_from_file(
      &scene->gpu, STR("res/textures/claymore-sword.png"), ErrPanic);
  sandbox->texture[1] = cm_texture_from_file(
      &scene->gpu, STR("res/textures/mushroom.png"), ErrPanic);

  cm_scene_push(scene, fps_scene_init);
}

static void update(CmScene *scene, double dt) {
  (void)dt;
  Sandbox *sandbox = scene->data;

  cm_renderer2d_begin(&sandbox->camera);
  {
    const vec2 size = {100.f, 100.f};
    const f32 margin = 10.f;
    for (usize i = 0; i < 4; ++i) {
      float y = (size[1] + margin) * i;
      for (usize j = 0; j < 4; ++j) {
        float x = (size[0] + margin) * j;
        cm_sprite_push(&sandbox->texture[(i + j) % ARRAY_LEN(sandbox->texture)],
                       (vec2){x, y}, size, 0, (vec2){0}, (vec2){1, 1});
      }
    }

    Str msg = STR("This is Claymore!");
    const float font_size = 32;
    const float char_width = 13;
    cm_quad_push(sandbox->mouse_pos,
                 (vec2){msg.len * char_width, font_size + margin}, 0,
                 (vec4){1, 0, 0, 1});

    cm_font_draw(sandbox->font, sandbox->mouse_pos, msg);
  }
  cm_renderer2d_end();
}

static CmSceneInterface *sandbox(void) {
  static CmSceneInterface interface = {
      .init = init,
      .update = update,
      .event = event,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 720, .height = 420, .title = "sandbox"},
      .main = sandbox,
  };
  return &config;
}
