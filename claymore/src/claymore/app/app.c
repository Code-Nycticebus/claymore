#include "app.h"

#include "claymore/renderer/context.h"
#include "sound.h"

// internal app instance
static CmAppInternal *app;

/* ========= app public ========= */

CmApp *cm_app(void) { return &app->public; }

RGFW_window *cm_app_window(void) { return app->public.window; }

float cm_app_aspect(void) {
  RGFW_window *w = app->public.window;
  return (float)w->r.w / (float)w->r.h;
}

void cm_app_background(const vec3 color) { glClearColor(VEC3_ARG(color), 1); }

CmGpu *cm_app_gpu(void) { return &app->public.gpu; }

CmScene *cm_app_root(void) { return &app->root->public; }
CmScene *cm_app_set_root(CmSceneInit init) {
  // initalizing new root scene.
  // for a short time there are gonna be two scenes in memory at the same time.

  app->new_root = cm_scene_internal_init(&app->arena, init);
  if (app->new_root->interface->init) {
    app->new_root->interface->init(&app->new_root->public);
  }
  return (CmScene *)app->new_root;
}

void *cm_app_data(void) {
  cebus_assert(app->data, "app data was never set");
  return app->data;
}

void *cm_app_set_data(usize size) {
  cebus_assert(app->data == NULL, "Trying to set app data twice!");
  app->data = arena_calloc(&app->arena, size);
  return app->data;
}

double cm_app_deltatime(void) { return app->deltatime; }
u64 cm_app_time(void) { return RGFW_getTimeNS() - app->first_frame; }

void cm_app_quit(i32 code) {
  cm_event_emit((CmEvent){
      .type = CM_EVENT_QUIT,
      .event.quit = {.code = code},
  });
}

/* ================== app internal ================== */

static void _cm_app_scene_fixed_update(CmSceneInternal *root) {
  if (root->interface->fixed_update) {
    root->interface->fixed_update(&root->public);
  }
  for (usize i = 0; i < da_len(&root->children); ++i) {
    _cm_app_scene_fixed_update(da_get(&root->children, i));
  }
}

static void _cm_app_scene_frame_update(CmSceneInternal *root) {
  if (root->interface->pre_update) {
    root->interface->pre_update(&root->public);
  }
  if (root->interface->frame_update) {
    root->interface->frame_update(&root->public);
  }
  for (usize i = 0; i < da_len(&root->children); ++i) {
    _cm_app_scene_frame_update(da_get(&root->children, i));
  }
  if (root->interface->post_update) {
    root->interface->post_update(&root->public);
  }
}

bool cm_app_internal_init(ClaymoreConfig *config) {
  app = calloc(1, sizeof(CmAppInternal));

  app->first_frame = RGFW_getTimeNS();
  app->last_frame = app->first_frame;

  RGFW_rect r = RGFW_RECT(0, 0, config->window.width, config->window.height);
  app->public.window =
      RGFW_createWindow(config->window.title, r, config->window.args);
  if (app->public.window == NULL) {
    cebus_log_error("Could not open window");
    return false;
  }

  if (!cm_platform_context_init(app->public.window)) {
    cebus_log_error("Could not initialize context");
    return false;
  }
#ifdef CLAYMORE_DEBUG
  cebus_log_info("OpenGl %s", glGetString(GL_VERSION));
  cebus_log_info("GPU: %s", glGetString(GL_RENDERER));
  cebus_log_info("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif

  if (!cm_sound_internal_init()) {
    cebus_log_error("Could not initialize sound engine");
    return false;
  }

  da_init(&app->deleted, &app->arena);

  app->renderer = cm_2D_internal_init();

  app->root = cm_scene_internal_init(&app->arena, config->root);
  if (!app->root->interface->init) {
    cebus_log_error("Main CmSceneInterface needs an init function");
    return false;
  }

  app->public.gpu = cm_gpu_internal_init(&app->arena);

  app->root->interface->init(&app->root->public);
  app->running = true;
  return true;
}

bool cm_app_internal_update(void) {
  // complete the new root initalization
  if (app->new_root) {
    da_push(&app->deleted, app->root);
    app->root = app->new_root;
    app->new_root = NULL;
  }

  // delete all scheduled scenes
  while (da_len(&app->deleted)) {
    CmSceneInternal *scene = da_pop(&app->deleted);
    // if there is no parent the scene was the root scene
    // deallocate from app arena
    Arena *arena = scene->parent ? &scene->parent->arena : &app->arena;
    cm_scene_internal_final(arena, scene);
  }

  cm_event_internal_poll_events(app->public.window);

  if (!app->running) {
    return false;
  }

  // deltatime
  const double ns = 1e+9;
  const u64 current_time = RGFW_getTimeNS();
  u64 dt = current_time - app->last_frame;
  app->last_frame = current_time;
  app->deltatime = dt / ns;

  // fixed Update
  const i64 fixed_interval = CM_FIXED_DELTA * ns;
  static i64 fixed_timer = 0;
  fixed_timer += dt;
  while (fixed_interval <= fixed_timer) {
    _cm_app_scene_fixed_update(app->root);
    fixed_timer -= fixed_interval;
  }

  // rendering
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _cm_app_scene_frame_update(app->root);

  RGFW_window_swapBuffers(app->public.window);
  return true;
}

void cm_app_internal_final(void) {
  cm_scene_internal_final(&app->arena, app->root);
  cm_sound_interal_shutdown();
  cm_2D_internal_free();

  RGFW_window_close(app->public.window);
  arena_free(&app->arena);
  free(app);
}

void cm_app_internal_set_context(CmApp *new_app) {
  cebus_assert(app == NULL, "cannot use and app while another app is running!");
  cebus_assert(new_app != NULL, "new app is not initialized");
  app = (CmAppInternal *)new_app;
  cm_2D_internal_set_context(app->renderer);
  cm_platform_context_init(app->public.window);
}

void cm_app_internal_event(CmEvent *event) {
  cm_scene_internal_event(app->root, event);
  if (!event->handled && event->type == CM_EVENT_QUIT) {
    app->running = false;
  }
}

void cm_app_internal_schedule_delete(CmScene *scene) {
  da_push(&app->deleted, (CmSceneInternal *)scene);
}
