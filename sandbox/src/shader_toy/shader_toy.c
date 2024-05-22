#include "claymore/entrypoint.h"

#include "shader_toy.h"

typedef struct {
  Error error;
  CmScene *child;
  Str filename;
} Toy;

static void load_shader_scene(CmScene *scene) {
  Toy *toy = scene->data;
  toy->child = shader_init(scene, toy->filename, &toy->error);
  error_context(&toy->error, {
    cm_scene_delete(scene, toy->child);

    toy->child =
        error_display_init(scene, str_copy(error_msg(), &scene->arena));
    error_except();
  });
}

static void toy_init(CmScene *scene) {
  Toy *toy = cm_scene_alloc_data(scene, sizeof(Toy));

  toy->filename = STR("assets/shader/toy.fs.glsl");
  load_shader_scene(scene);
}

static void event(CmScene *scene, CmEvent *event) {
  Toy *toy = scene->data;
  cm_event_key(event, {
    if (key->action == CM_KEY_RELEASE) {
      if (key->code == CM_KEY_ESCAPE) {
        cm_window_close();
        return;
      }
      if (key->code == CM_KEY_F5) {
        cm_scene_delete(scene, toy->child);
        load_shader_scene(scene);
      }
    }
  });
}

static CmSceneInterface *toy(void) {
  static CmSceneInterface interface = {
      .init = toy_init,
      .event = event,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 720, .height = 420, .title = "Shader Toy"},
      .main = toy,
  };
  return &config;
}
