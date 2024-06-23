#include "app.h"

#include "claymore/renderer/2D/renderer2D.h"
#include "claymore/renderer/context.h"
#include "sound.h"

// Internal app context
typedef struct {
  // Public app context
  CmApp public;
  // App lifetime arena
  Arena arena;
  // Running state
  bool running;
  // Timestamp of first frame
  u64 first_frame;
  // Deltatime
  u64 last_frame;
  double deltatime;
  // Current root scene
  CmSceneInternal *root;
  // Scene management
  CmSceneInternal *new_root;
  DA(CmSceneInternal *) deleted;
  // Flat scene tree
  bool update_scene_flat;
  DA(CmSceneInternal *) flat;
} CmAppInternal;

// Internal app instance
static CmAppInternal *app;

/* ========= App public ========= */

CmApp *cm_app(void) { return &app->public; }

RGFW_window *cm_app_window(void) { return app->public.window; }

void cm_app_background(const vec3 color) { glClearColor(VEC3_ARG(color), 1); }

CmGpu *cm_app_gpu(void) { return &app->public.gpu; }

CmScene *cm_app_root(void) { return &app->root->public; }
CmScene *cm_app_set_root(CmSceneInit init) {
  // Initalizing new root scene.
  // For a short time there are gonna be two scenes running at the same time.

  app->new_root = cm_scene_internal_init(&app->arena, init);
  if (app->new_root->interface->init) {
    app->new_root->interface->init(&app->new_root->public);
  }
  return (CmScene *)app->new_root;
}

double cm_app_deltatime(void) { return app->deltatime; }
u64 cm_app_time(void) { return RGFW_getTimeNS() - app->first_frame; }

void cm_app_quit(void) {
  cm_event_emit((CmEvent){
      .type = CM_EVENT_QUIT,
      .event = {{0}},
  });
}

/* ================== App internal ================== */

// Build flat scene tree
static void _cm_app_build_flat_tree(CmSceneInternal *root) {
  da_extend(&app->flat, da_len(&root->children), root->children.items);
  for (usize i = 0; i < da_len(&root->children); ++i) {
    _cm_app_build_flat_tree(da_get(&root->children, i));
  }
}

bool cm_app_internal_init(ClaymoreConfig *config) {
  app = calloc(1, sizeof(CmAppInternal));

  app->first_frame = RGFW_getTimeNS();
  app->last_frame = app->first_frame;

  RGFW_rect r = RGFW_RECT(0, 0, config->window.width, config->window.height);
  app->public.window = RGFW_createWindow(config->window.title, r, 0);
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

  cm_2D_internal_init();

  app->root = cm_scene_internal_init(&app->arena, config->root);
  if (!app->root->interface->init) {
    cebus_log_error("Main CmSceneInterface needs an init function");
    return false;
  }

  app->public.gpu = cm_gpu_internal_init(&app->arena);

  da_init(&app->flat, &app->arena);
  app->update_scene_flat = true;

  app->root->interface->init(&app->root->public);
  app->running = true;
  return true;
}

bool cm_app_internal_update(void) {
  // Complete the new root initalization
  if (app->new_root) {
    da_push(&app->deleted, app->root);
    app->root = app->new_root;
    app->new_root = NULL;
    app->update_scene_flat = true;
  }

  // Delete all scheduled scenes
  while (da_len(&app->deleted)) {
    CmSceneInternal *scene = da_pop(&app->deleted);
    Arena *arena = scene->parent ? &scene->parent->arena : &app->arena;
    cm_scene_internal_final(arena, scene);
  }

  // Update flat scene tree if needed
  if (app->update_scene_flat) {
    da_clear(&app->flat);
    da_push(&app->flat, app->root);
    _cm_app_build_flat_tree(app->root);
    app->update_scene_flat = false;
  }

  cm_event_internal_poll_events(app->public.window);

  if (!app->running) {
    return false;
  }

  // Deltatime
  const double ns = 1e+9;
  const u64 current_time = RGFW_getTimeNS();
  u64 dt = current_time - app->last_frame;
  app->last_frame = current_time;
  app->deltatime = dt / ns;

  // Fixed Update
  const i64 fixed_interval = CM_FIXED_DELTA * ns;
  static i64 fixed_timer = 0;
  fixed_timer += dt;
  while (fixed_interval <= fixed_timer) {
    for (usize i = 0; i < da_len(&app->flat); ++i) {
      CmSceneInternal *scene = da_get(&app->flat, i);
      if (scene->interface->fixed_update) {
        scene->interface->fixed_update(&scene->public);
      }
    }

    fixed_timer -= fixed_interval;
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (usize i = 0; i < da_len(&app->flat); ++i) {
    CmSceneInternal *scene = da_get(&app->flat, i);
    // Pre update
    if (scene->interface->pre_update) {
      scene->interface->pre_update(&scene->public);
    }
    // Frame update
    if (scene->interface->frame_update) {
      scene->interface->frame_update(&scene->public);
    }
    // Post update
    if (scene->interface->post_update) {
      scene->interface->post_update(&scene->public);
    }
  }

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

void cm_app_internal_event(CmEvent *event) {
  cm_scene_internal_event(app->root, event);
  if (!event->handled && event->type == CM_EVENT_QUIT) {
    app->running = false;
  }
}

void cm_app_internal_schedule_delete(CmScene *scene) {
  da_push(&app->deleted, (CmSceneInternal *)scene);
  app->update_scene_flat = true;
}

void cm_app_internal_schedule_build(void) { app->update_scene_flat = true; }
