#include "claymore/entrypoint.h"

#include "claymore.h"

typedef struct {
  CmMesh mesh;
  CmVbo pos;
  CmVbo rot;
  CmShader shader;
} Sandbox;

vec3 vertices[4] = {
    {-1.f, 1.f, .0f},
    {-1.f, .0f, .0f},
    {0.0f, .0f, .0f},
    {0.0f, 1.f, .0f},
};

u32 indices[6] = {
    0, 1, 3, 1, 2, 3,
};

vec3 positions[] = {
    {-0.5f, -0.5f, 0.0f},
    {.5, 0.0f, 0.0f},
    {0.75f, 0.25f, 0.0f},
};

vec2 scales[] = {
    {1.00f, 1.00f},
    {0.50f, 0.50f},
    {0.25f, 0.25f},
};

vec3 colors[] = {
    {0.f, 0.f, 1.f},
    {0.f, 1.f, 0.f},
    {1.f, 0.f, 0.f},
};

vec4 color1 = {1, 1, 1, 1};
vec4 color2 = {1, 1, 1, .5f};

static void sandbox_init(CmScene *scene) {
  clib_log_info("sandbox init");
  Sandbox *sandbox = arena_alloc(&scene->arena, sizeof(Sandbox));

  sandbox->mesh = cm_mesh_create(&scene->gpu, 4, vertices);
  cm_mesh_attach_ebo(&sandbox->mesh, 6, indices);
  sandbox->pos = cm_mesh_attach_vec3_instanced(&sandbox->mesh, 3, positions);
  cm_mesh_attach_vec2_instanced(&sandbox->mesh, 3, scales);

  float rotations[] = {
      glm_rad(0),
      glm_rad(45),
      glm_rad(30),
  };
  sandbox->rot = cm_mesh_attach_f32_instanced(&sandbox->mesh, 3, rotations);

  cm_mesh_attach_vec3_instanced(&sandbox->mesh, 3, colors);

  sandbox->shader = cm_shader_from_memory( // Format like this you idiot
      STR("#version 330 core\n"
          "layout (location = 0) in vec3 a_pos;\n"
          "layout (location = 1) in vec3 a_translation;\n"
          "layout (location = 2) in vec2 a_scale;\n"
          "layout (location = 3) in float a_rotation;\n"
          "layout (location = 4) in vec3 a_color;\n"

          "vec3 rotateZ(vec3 v, float angle) {\n"
          "  float s = sin(angle);\n"
          "  float c = cos(angle);\n"
          "  mat3 rotationMatrix = mat3(\n"
          "    vec3(c, -s, 0.0),\n"
          "    vec3(s, c, 0.0),\n"
          "    vec3(0.0, 0.0, 1.0)\n"
          "  );\n"
          "  return rotationMatrix * v;\n"
          "}\n"

          "out vec4 v_color;\n"
          "void main() {\n"
          "  vec3 pos = rotateZ(a_pos, a_rotation) * vec3(a_scale, 1.0) + "
          "a_translation;\n"
          "  gl_Position = vec4(pos.xyz, 1.0);\n"
          "  v_color = vec4(a_color, 1.0);\n"
          "}\n"),
      STR("#version 330 core\n"
          "in vec4 v_color;\n"
          "out vec4 f_color;\n"
          "uniform vec4 u_color = vec4(1,0,0,1);\n"
          "void main() {\n"
          "  f_color = v_color * u_color;\n"
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

  for (usize i = 0; i < 3; ++i) {
    float val = sinf(cm_window_time() * (i + 1));
    positions[i][1] = -val;
  }
  cm_gpu_vbo_update(&sandbox->pos, 3, sizeof(vec3), &positions[0][0]);

  static float r = 0;
  r += (3.14 / 4) * deltatime;
  cm_gpu_vbo_update(&sandbox->rot, 1, sizeof(float), &r);

  cm_mesh_draw_indexed(&sandbox->mesh, CM_DRAW_TRIANGLES);
}

static void sandbox_free(CmScene *scene) {
  (void)scene;
  clib_log_info("sandbox free");
}

static void sandbox_on_event(CmScene *scene, CmEvent *event) {
  Sandbox *sandbox = scene->data;
  (void)sandbox;

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
    clib_log_info("Scroll Event: " VEC2_FMT, VEC2_ARG(scroll->offset));
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
