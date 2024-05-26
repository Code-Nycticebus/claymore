#include "app.h"

#include "claymore/renderer/2D/renderer2D.h"
#include "claymore/renderer/context.h"
#include "sound.h"

static struct {
  CmApp data;
  bool running;
  double last_frame;
  CmSceneInternal *main_scene;
} app;

CmApp *cm_app(void) { return &app.data; }
CmScene *cm_app_root(void) { return &app.main_scene->data; }
RGFW_window *cm_app_window(void) { return app.data.window; }
Arena *cm_app_arena(void) { return &app.data.arena; }
void *cm_app_alloc(usize size) { return arena_calloc(&app.data.arena, size); }

void cm_app_set_main(CmSceneInit init) {
  // Free previous main scene
  cm_scene_internal_final(&app.data.arena, app.main_scene);

  // Initialize new one
  app.main_scene = cm_scene_internal_init(&app.data.arena, init);
  if (!app.main_scene->interface->init) {
    cebus_log_error("Main CmSceneInterface needs an init function");
    DEBUGBREAK();
  }
  app.main_scene->interface->init(&app.main_scene->data);
}

double cm_app_time(void) {
  const double ns = 1e9;
  return RGFW_getTimeNS() / ns;
}

void cm_app_quit(void) {
  cm_event_emit((CmEvent){
      .type = CM_EVENT_QUIT,
      .event = {{0}},
  });
}

void cm_app_background(const vec3 color) { glClearColor(VEC3_ARG(color), 1); }

// INTERNAL

bool cm_app_internal_init(ClaymoreConfig *config) {
  app.data.window = RGFW_createWindow(
      config->window.title,
      RGFW_RECT(0, 0, config->window.width, config->window.height), 0);
  if (app.data.window == NULL) {
    cebus_log_error("Could not open window");
    return false;
  }

  if (!cm_sound_internal_init()) {
    cebus_log_error("Could not initialize sound engine");
    return false;
  }

  if (!cm_platform_context_init(app.data.window)) {
    cebus_log_error("Could not initialize context");
    return false;
  }

  cm_2D_internal_init();

  app.main_scene = cm_scene_internal_init(&app.data.arena, config->main);
  if (!app.main_scene->interface->init) {
    cebus_log_error("Main CmSceneInterface needs an init function");
    return false;
  }
  app.main_scene->interface->init(&app.main_scene->data);
  app.last_frame = cm_app_time();
  app.running = true;
  return true;
}

bool cm_app_internal_update(void) {
  cm_event_internal_poll_events(app.data.window);

  if (!app.running) {
    return false;
  }

  double current_time = cm_app_time();
  double dt = current_time - app.last_frame;
  app.last_frame = current_time;

  const float fixed_interval = 1.f / 60.f;
  static float fixed_timer = 0;
  fixed_timer += dt;
  while (fixed_interval <= fixed_timer) {
    cm_scene_internal_fixed_update(app.main_scene, fixed_interval);
    fixed_timer -= fixed_interval;
  }

  glClear(GL_COLOR_BUFFER_BIT);
  cm_scene_internal_frame_update(app.main_scene, dt);

  RGFW_window_swapBuffers(app.data.window);
  return true;
}

void cm_app_internal_final(void) {
  cm_scene_internal_final(&app.data.arena, app.main_scene);

  cm_sound_interal_shutdown();

  cm_2D_internal_free();

  RGFW_window_close(app.data.window);
  arena_free(&app.data.arena);
}

void cm_app_internal_event(CmEvent *event) {
  cm_scene_internal_event(app.main_scene, event);
  if (!event->handled && event->type == CM_EVENT_QUIT) {
    app.running = false;
  }
}
