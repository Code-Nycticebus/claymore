#include "app.h"

#include "claymore/renderer/2D/renderer2D.h"
#include "claymore/renderer/context.h"
#include "sound.h"

static struct {
  CmApp public;
  Arena arena;
  bool running;
  u64 first_frame;
  u64 last_frame;
  CmSceneInternal *root;
  CmSceneInternal *new_root;
  DA(CmSceneInternal *) deleted;

  bool update_scene_flat;
  DA(CmSceneInternal *) flat;

  double deltatime;
} app;

CmApp *cm_app(void) { return &app.public; }
CmScene *cm_app_root(void) { return &app.root->public; }
RGFW_window *cm_app_window(void) { return app.public.window; }
void *cm_app_data(void) { return app.public.data; }
CmGpu *cm_app_gpu(void) { return &app.public.gpu; }

void *cm_app_set_data(usize size) {
  cebus_assert(app.public.data == NULL, "Trying to set app data twice");
  app.public.data = arena_calloc(&app.arena, size);
  return app.public.data;
}

double cm_app_deltatime(void) { return app.deltatime; }

CmScene *cm_app_set_main(CmSceneInit init) {
  app.new_root = cm_scene_internal_init(&app.arena, init);
  if (app.new_root->interface->init) {
    app.new_root->interface->init(&app.new_root->public);
  }
  return (CmScene *)app.new_root;
}

u64 cm_app_time(void) { return RGFW_getTimeNS() - app.first_frame; }

void cm_app_quit(void) {
  cm_event_emit((CmEvent){
      .type = CM_EVENT_QUIT,
      .event = {{0}},
  });
}

void cm_app_background(const vec3 color) { glClearColor(VEC3_ARG(color), 1); }

// INTERNAL

static void cm_app_build_flat(CmSceneInternal *root) {
  da_extend(&app.flat, da_len(&root->children), root->children.items);
  for (usize i = 0; i < da_len(&root->children); ++i) {
    cm_app_build_flat(da_get(&root->children, i));
  }
}

bool cm_app_internal_init(ClaymoreConfig *config) {
  app.first_frame = RGFW_getTimeNS();
  app.last_frame = app.first_frame;

  app.public.window = RGFW_createWindow(
      config->window.title,
      RGFW_RECT(0, 0, config->window.width, config->window.height), 0);
  if (app.public.window == NULL) {
    cebus_log_error("Could not open window");
    return false;
  }

  if (!cm_sound_internal_init()) {
    cebus_log_error("Could not initialize sound engine");
    return false;
  }

  if (!cm_platform_context_init(app.public.window)) {
    cebus_log_error("Could not initialize context");
    return false;
  }

  da_init(&app.deleted, &app.arena);

  cm_2D_internal_init();

  app.root = cm_scene_internal_init(&app.arena, config->root);
  if (!app.root->interface->init) {
    cebus_log_error("Main CmSceneInterface needs an init function");
    return false;
  }

  app.public.gpu = cm_gpu_internal_init(&app.arena);

  da_init(&app.flat, &app.arena);
  app.update_scene_flat = true;

  app.root->interface->init(&app.root->public);
  app.running = true;
  return true;
}

bool cm_app_internal_update(void) {
  if (app.new_root) {
    da_push(&app.deleted, app.root);
    app.root = app.new_root;
    app.new_root = NULL;
    app.update_scene_flat = true;
  }

  while (da_len(&app.deleted)) {
    CmSceneInternal *scene = da_pop(&app.deleted);
    Arena *arena = scene->parent ? &scene->parent->arena : &app.arena;
    cm_scene_internal_final(arena, scene);
  }

  if (app.update_scene_flat) {
    da_clear(&app.flat);
    da_push(&app.flat, app.root);
    cm_app_build_flat(app.root);
    app.update_scene_flat = false;
  }

  cm_event_internal_poll_events(app.public.window);

  if (!app.running) {
    return false;
  }

  const double ns = 1e+9;
  const u64 current_time = RGFW_getTimeNS();
  u64 dt = current_time - app.last_frame;
  app.last_frame = current_time;
  app.deltatime = dt / ns;

  const i64 fixed_interval = CM_FIXED_DELTA * ns;
  static i64 fixed_timer = 0;
  fixed_timer += dt;
  while (fixed_interval <= fixed_timer) {
    for (usize i = 0; i < da_len(&app.flat); ++i) {
      CmSceneInternal *scene = da_get(&app.flat, i);
      if (scene->interface->fixed_update) {
        scene->interface->fixed_update(&scene->public);
      }
    }

    fixed_timer -= fixed_interval;
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (usize i = 0; i < da_len(&app.flat); ++i) {
    CmSceneInternal *scene = da_get(&app.flat, i);
    if (scene->interface->pre_update) {
      scene->interface->pre_update(&scene->public);
    }
    if (scene->interface->frame_update) {
      scene->interface->frame_update(&scene->public);
    }
    if (scene->interface->post_update) {
      scene->interface->post_update(&scene->public);
    }
  }

  RGFW_window_swapBuffers(app.public.window);
  return true;
}

void cm_app_internal_final(void) {
  cm_scene_internal_final(&app.arena, app.root);

  cm_sound_interal_shutdown();

  cm_2D_internal_free();

  RGFW_window_close(app.public.window);
  arena_free(&app.arena);
}

void cm_app_internal_event(CmEvent *event) {
  cm_scene_internal_event(app.root, event);
  if (!event->handled && event->type == CM_EVENT_QUIT) {
    app.running = false;
  }
}

void cm_app_internal_schedule_delete(CmScene *scene) {
  da_push(&app.deleted, (CmSceneInternal *)scene);
  app.update_scene_flat = true;
}

void cm_app_internal_schedule_build(void) { app.update_scene_flat = true; }
