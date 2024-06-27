#include "claymore/entrypoint.h"

#include "utils/fps.h"
#include "utils/gui.h"

CmSceneInterface *sandbox(void);

static void _event(CmScene *scene, CmEvent *event) {
  cm_event_key(event, {
    if (key->action == RGFW_keyPressed && key->code == RGFW_Escape) {
      cm_app_set_root(sandbox);
      event->handled = true;
    }
  });

  cm_event_key(event, {
    if (key->action == RGFW_keyPressed && key->code == RGFW_F1) {
      CmScene *f = cm_scene_find(scene, STR("Fps"));
      if (f == NULL) {
        const vec2 pos = {10, 0};
        const Str font = STR_STATIC("assets/fonts/Ubuntu.ttf");
        const float font_size = 32.f;
        fps(scene, pos, font, font_size);
      } else {
        cm_scene_delete(f);
      }
    }
  });
}

CmSceneInterface *app_controlls(void) {
  static CmSceneInterface interface = {
      .event = _event,
  };
  return &interface;
}

CmSceneInterface *benchmark(void);
CmSceneInterface *counter(void);
CmSceneInterface *test(void);
CmSceneInterface *hello(void);

static const struct {
  Str label;
  CmSceneInit scene;
} buttons[] = {
    {.label = STR_STATIC("benchmark"), .scene = benchmark},
    {.label = STR_STATIC("counter"), .scene = counter},
    {.label = STR_STATIC("hello"), .scene = hello},
    {.label = STR_STATIC("test"), .scene = test},
};

static void gui_update(CmScene *scene) {
  for (usize i = 0; i < ARRAY_LEN(buttons); ++i) {
    if (button(scene, buttons[i].label)) {
      CmScene *m = cm_app_set_root(buttons[i].scene);
      cm_scene_push(m, app_controlls);
    }
  }

  if (button(scene, STR("quit"))) {
    cm_app_quit(0);
  }
}

static void event(CmScene *scene, CmEvent *event) {
  (void)scene;
  cm_event_key(event, {
    if (key->action == RGFW_keyPressed) {
      if (key->code == RGFW_Escape) {
        cm_app_quit(0);
      }
    }
  });
}

static void init(CmScene *scene) {
  const float width = 250.f;

  const vec3 bg = {.1f, .1f, .1f};
  cm_app_background(bg);

  RGFW_window *w = cm_app_window();
  const float y = 50.f;
  gui_init(scene, (vec2){w->r.w / 2 - width / 2, y}, width, gui_update);
}

CmSceneInterface *sandbox(void) {
  static CmSceneInterface interface = {
      .init = init,
      .event = event,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 720, .height = 420, .title = "Sandbox"},
      .root = sandbox,
  };
  return &config;
}
