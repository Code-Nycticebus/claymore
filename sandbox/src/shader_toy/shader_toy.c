#include "claymore/entrypoint.h"

#include "shader_toy.h"

#include <sys/stat.h>
#if defined(LINUX)
#define MTIME(s) (s).st_mtim.tv_sec
#elif defined(WINDOWS)
#define MTIME(s) (s).st_mtime
#endif

typedef struct {
  Error error;
  CmScene *child;
  Str filename;
} Toy;

static void load_shader_scene(CmScene *scene) {
  Toy *toy = scene->data;
  if (toy->child != NULL) {
    cm_scene_delete(scene, toy->child);
  }
  // Reload
  toy->child = shader_init(scene, toy->filename, &toy->error);
  error_context(&toy->error, {
    cm_scene_delete(scene, toy->child);
    toy->child = error_display_init(scene, error_msg());
    error_except();
  });
}

static void init(CmScene *scene) {
  Toy *toy = cm_scene_alloc_data(scene, sizeof(Toy));

  // keep file watcher in mind!
  toy->filename = STR("assets/shader/toy.fs.glsl");
}

static void event(CmScene *scene, CmEvent *event) {
  (void)scene;
  cm_event_key(event, {
    if (key->action == RGFW_keyPressed) {
      if (key->code == RGFW_Escape) {
        cm_app_quit();
        return;
      }
    }
  });
}

static void update(CmScene *scene, double dt) {
  Toy *toy = scene->data;
  const float interval = 1.0f;
  static float timer = interval;
  timer += dt;
  if (interval < timer) {
    timer = 0;
    static int last = 0;
    struct stat file;
    stat(toy->filename.data, &file);
    int mtime = MTIME(file);
    if (last != mtime) {
      last = mtime;
      load_shader_scene(scene);
    }
  }
}

static CmSceneInterface *toy(void) {
  static CmSceneInterface interface = {
      .init = init,
      .update = update,
      .event = event,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 720, .height = 720, .title = "Shader Toy"},
      .main = toy,
  };
  return &config;
}
