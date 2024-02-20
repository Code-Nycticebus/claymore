#include "claymore/entrypoint.h"

#include "claymore.h"

typedef struct {
  CmMesh mesh;
  CmVbo pos;
  CmVbo rot;
  CmShader shader;
  mat4 view;
  mat4 projection;
} Sandbox;

vec3 vertices[4] = {
    {0.0f, 1.f, .0f},
    {0.0f, 0.0f, .0f},
    {1.0f, 0.0f, .0f},
    {1.0f, 1.0f, .0f},
};

u32 indices[6] = {
    0, 1, 3, 1, 2, 3,
};

#define SANDBOX_MAX_QUADS 20000

vec3 positions[SANDBOX_MAX_QUADS] = {0};
vec2 scales[SANDBOX_MAX_QUADS] = {0};
vec3 colors[SANDBOX_MAX_QUADS] = {0};
float rotations[SANDBOX_MAX_QUADS] = {0};

static float rand_f32(void) { return (float)rand() / (float)RAND_MAX; }

static void sandbox_init(CmScene *scene) {
  clib_log_info("sandbox init");
  Sandbox *sandbox = arena_alloc(&scene->arena, sizeof(Sandbox));

  for (usize i = 0; i < 4; i++) {
    glm_vec3_add(vertices[i], (vec3){-.5f, -.5f, 0}, vertices[i]);
  }
  sandbox->mesh = cm_mesh_create(&scene->gpu, 4, vertices);

  for (usize i = 0; i < SANDBOX_MAX_QUADS; i++) {
    positions[i][0] = rand_f32() * 720;
  }
  sandbox->pos = cm_mesh_attach_vec3_instanced(&sandbox->mesh,
                                               SANDBOX_MAX_QUADS, positions);

  for (usize i = 0; i < SANDBOX_MAX_QUADS; i++) {
    float scale = rand_f32() * 200;
    scales[i][0] = scale;
    scales[i][1] = scale;
  }
  cm_mesh_attach_vec2_instanced(&sandbox->mesh, SANDBOX_MAX_QUADS, scales);

  for (usize i = 0; i < SANDBOX_MAX_QUADS; i++) {
    rotations[i] = rand_f32() * 3.141f;
  }
  sandbox->rot = cm_mesh_attach_f32_instanced(&sandbox->mesh, SANDBOX_MAX_QUADS,
                                              rotations);

  for (usize i = 0; i < SANDBOX_MAX_QUADS; i++) {
    colors[i][0] = rand_f32();
    colors[i][1] = rand_f32();
    colors[i][2] = rand_f32();
  }
  cm_mesh_attach_vec3_instanced(&sandbox->mesh, SANDBOX_MAX_QUADS, colors);

  cm_mesh_attach_ebo(&sandbox->mesh, 6, indices);

  sandbox->shader = cm_shader_from_memory( // Format like this you idiot
      STR("#version 330 core\n"
          "layout (location = 0) in vec3 a_pos;\n"
          "layout (location = 1) in vec3 a_translation;\n"
          "layout (location = 2) in vec2 a_scale;\n"
          "layout (location = 3) in float a_rotation;\n"
          "layout (location = 4) in vec3 a_color;\n"

          "uniform mat4 u_mvp;"

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
          "  vec3 pos = rotateZ(a_pos, a_rotation) * vec3(a_scale, 1.0);\n"
          "  pos += a_translation;\n"
          "  gl_Position = u_mvp * vec4(pos.xyz, 1.0);\n"
          "  v_color = vec4(a_color, 1.0);\n"
          "}\n"),
      STR("#version 330 core\n"
          "in vec4 v_color;\n"
          "out vec4 f_color;\n"
          "void main() {\n"
          "  f_color = v_color;\n"
          "}\n"),
      ErrPanic);

  cm_window_bg_color((vec3){0.15f, 0.15f, 0.15f});

  glm_ortho(0, 720, 0, 420, -1, 100.f, sandbox->view);
  glm_mat4_identity(sandbox->projection);

  mat4 vp;
  glm_mat4_mul(sandbox->view, sandbox->projection, vp);
  cm_shader_set_mat4(&sandbox->shader, STR("u_mvp"), vp);

  scene->data = sandbox;
}

static void sandbox_update(CmScene *scene, double deltatime) {
  clib_log_info("fps: %f", 1 / deltatime);

  Sandbox *sandbox = scene->data;

  cm_shader_bind(&sandbox->shader);

  // mat4 mvp = GLM_MAT4_IDENTITY_INIT;
  // mat4 vp;
  // glm_mat4_mul(sandbox->view, sandbox->projection, vp);
  // cm_shader_set_mat4(&sandbox->shader, STR("u_mvp"), vp);

  for (usize i = 0; i < SANDBOX_MAX_QUADS; ++i) {
    float val = sinf(cm_window_time() * i / SANDBOX_MAX_QUADS) * 0.5f + 0.5f;
    positions[i][1] = val * 420 / 2 + (420.f / 4);
  }
  cm_gpu_vbo_update(&sandbox->pos, SANDBOX_MAX_QUADS, sizeof(vec3),
                    &positions[0][0]);

  for (usize i = 0; i < SANDBOX_MAX_QUADS; i++) {
    rotations[i] += (3.14 / 8) * deltatime;
  }
  cm_gpu_vbo_update(&sandbox->rot, SANDBOX_MAX_QUADS, sizeof(float),
                    &rotations[0]);

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
