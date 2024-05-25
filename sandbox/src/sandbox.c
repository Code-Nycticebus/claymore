#include "claymore/entrypoint.h"

#include "scenes/fps.h"

typedef struct {
  CmCamera2D camera;
  CmFont *font;
  CmTexture2D texture[2];
  vec2 mouse_pos;
  CmScene *overlay;
} Sandbox;

static void event(CmScene *scene, CmEvent *event) {
  Sandbox *sandbox = scene->data;
  (void)sandbox, (void)event;
  cm_event_key(event, {
    if (key->action == RGFW_keyPressed && key->code == RGFW_Escape) {
      cm_app_quit();
    }
    if (key->action == RGFW_keyPressed && key->code == RGFW_F1) {
      if (sandbox->overlay == NULL) {
        sandbox->overlay = cm_scene_push(scene, fps);
      } else {
        cm_scene_delete(scene, sandbox->overlay);
        sandbox->overlay = NULL;
      }
    }
  });
  cm_event_cursor(event, {
    sandbox->mouse_pos[0] = cursor->pos[0];
    sandbox->mouse_pos[1] = cursor->pos[1];
  });
}

static void init(CmScene *scene) {
  Sandbox *sandbox = cm_scene_set_data(scene, sizeof(Sandbox));

  cm_camera2D_screen(&sandbox->camera);

  const float font_size = 32.f;
  sandbox->font = cm_font_init(&scene->gpu, STR("assets/fonts/Ubuntu.ttf"),
                               font_size, ErrPanic);

  sandbox->texture[0] = cm_texture_from_file(
      &scene->gpu, STR("assets/textures/claymore-sword.png"), ErrPanic);
  sandbox->texture[1] = cm_texture_from_file(
      &scene->gpu, STR("assets/textures/mushroom.png"), ErrPanic);

  RGFW_registerJoystick(cm_app_window(), 0);
}

static void update(CmScene *scene, double dt) {
  (void)dt;
  Sandbox *sandbox = scene->data;

  cm_2D_begin(&sandbox->camera);
  {
    const vec2 size = {100.f, 100.f};
    const f32 margin = 10.f;
    for (usize i = 0; i < 4; ++i) {
      float y = (size[1] + margin) * i;
      for (usize j = 0; j < 4; ++j) {
        float x = (size[0] + margin) * j;
        cm_sprite(&sandbox->texture[(i + j) % ARRAY_LEN(sandbox->texture)],
                  (vec2){x, y}, size, 0, (vec2){0}, (vec2){1, 1});
      }
    }

    Str msg = STR("This is Claymore!");
    const float font_size = 32;
    const float char_width = 13;
    cm_quad(sandbox->mouse_pos,
            (vec2){msg.len * char_width, font_size + margin}, 0,
            (vec4){1, 0, 0, 1});

    RGFW_window *window = cm_app_window();
    vec2 pos = {window->r.w, window->r.h};
    glm_vec2_divs(pos, 2, pos);
    const vec2 r = {210, 210};

    cm_circle(pos, r, (vec4){0, 0, 1, 1});

    cm_font(sandbox->font, sandbox->mouse_pos, msg);
  }
  cm_2D_end();
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
