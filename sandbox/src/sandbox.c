#include "claymore/entrypoint.h"

#include "claymore.h"

typedef struct {
  CmMesh mesh;
  CmShader shader;
} Sandbox;

vec4 color1 = {1, 0, 0, 1};
vec4 color2 = {0, 0, 1, 1};

static void sandbox_init(CmScene *scene) {
  clib_log_info("sandbox init");
  Sandbox *sandbox = arena_alloc(&scene->arena, sizeof(Sandbox));

  vec3 vertices[4] = {
      {-1.f, 1.f, .0f},
      {-1.f, .8f, .0f},
      {-.8f, .8f, .0f},
      {-.8f, 1.f, .0f},
  };
  u32 indices[6] = {
      0, 1, 3, 1, 2, 3,
  };

  sandbox->mesh = cm_mesh_create(&scene->arena, vertices, 4);
  cm_mesh_attach_index_buffer(&sandbox->mesh, indices, 6);

  vec3 positions[3] = {
      {.0f, 0, 0},
      {.3f, 0, 0},
      {.6f, 0, 0},
  };
  cm_mesh_attach_vec3_instanced(&sandbox->mesh, positions, 3);

  sandbox->shader = cm_shader_load_from_memory(
      STR("#version 330 core\n"
          "layout (location = 0) in vec3 a_pos;\n"
          "layout (location = 1) in vec3 a_translations;\n"
          "void main() {\n"
          "  vec3 pos = a_pos + a_translations;"
          "  gl_Position = vec4(pos.xyz, 1.0);\n"
          "}\n"),
      STR("#version 330 core\n"
          "out vec4 f_color;\n"
          "uniform vec4 u_color = vec4(1,0,0,1);\n"
          "void main() {\n"
          "  f_color = u_color;\n"
          "}\n"),
      ErrPanic);

  cm_window_bg_color((vec3){0.15f, 0.15f, 0.15f});

  scene->data = sandbox;
}

static void sandbox_update(CmScene *scene, double deltatime) {
  (void)deltatime;

  Sandbox *sandbox = scene->data;

  cm_shader_bind(&sandbox->shader);
  vec4 color;
  glm_vec4_lerp(color1, color2, sinf(cm_window_time()) * 0.5 + 0.5, color);
  cm_shader_set_vec4(&sandbox->shader, STR("u_color"), color);

  cm_mesh_draw_indexed(&sandbox->mesh);
}

static void sandbox_free(CmScene *scene) {
  Sandbox *sandbox = scene->data;
  cm_mesh_delete(&sandbox->mesh);
  clib_log_info("sandbox free");
}

static void sandbox_on_event(CmScene *scene, CmEvent *event) {
  (void)scene;
  cm_event_key(event, {
    clib_log_info("Key Event: ");
    if (key->code == CM_KEY_ESCAPE) {
      cm_window_close(true);
    }
  });

  cm_event_cursor(event, {
    clib_log_info("Cursor Event: " VEC2_FMT, VEC2_ARG(cursor->pos));
  });

  cm_event_mouse(event, {
    clib_log_info("Mouse Event: " VEC2_FMT, VEC2_ARG(mouse->pos));
  });

  cm_event_scroll(event, {
    clib_log_info("Scroll Event " VEC2_FMT, VEC2_ARG(scroll->offset));
  });

  cm_event_resize(event, {
    clib_log_info("Resize Event " VEC2_FMT, VEC2_ARG(resize->size));
  });

  cm_event_drop(event, {
    clib_log_info("Drop event: ");
    for (usize i = 0; i < drop->count; i++) {
      clib_log_info("- %s", drop->files[i]);
    }
  });
}

static CmSceneInterface *sandbox_scene_init(void) {
  static CmSceneInterface sandbox = {
      .init = sandbox_init,
      .update = sandbox_update,
      .free = sandbox_free,
      .on_event = sandbox_on_event,
  };
  return &sandbox;
}

const ClaymoreConfig *claymore_init(void) {
  static const ClaymoreConfig config = {
      .window =
          {
              .width = 720,
              .height = 420,
              .title = "sandbox",
          },
      .scene = sandbox_scene_init,
  };
  return &config;
}
