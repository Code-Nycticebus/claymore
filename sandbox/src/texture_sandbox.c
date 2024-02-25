#include "claymore/entrypoint.h"

#include "claymore.h"

typedef struct {
  CmMesh mesh;
  CmVbo pos;
  CmShader shader;
  CmTexture2D texture;
} Sandbox;

vec3 vertices[4] = {
    {0.0f, 1.f, .0f},
    {0.0f, 0.0f, .0f},
    {1.0f, 0.0f, .0f},
    {1.0f, 1.0f, .0f},
};

vec2 uv[4] = {
    {0.0f, 1.f},
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
};

u32 indices[6] = {
    0, 1, 3, 1, 2, 3,
};

vec3 positions[] = {
    {-1.0f, -1.0f, 0.0f},
};

static void init(CmScene *scene) {
  clib_log_info("sandbox init");
  Sandbox *sandbox = arena_calloc(&scene->arena, sizeof(Sandbox));
  scene->data = sandbox;

  sandbox->mesh = cm_mesh_create(&scene->gpu, 4, vertices);
  sandbox->pos = cm_mesh_attach_vec3_instanced(&sandbox->mesh, 1, positions);
  cm_mesh_attach_vec2(&sandbox->mesh, 4, uv);

  cm_mesh_attach_ebo(&sandbox->mesh, 6, indices);

  sandbox->shader = cm_shader_from_memory( // Format like this you idiot
      STR("#version 330 core\n"
          "layout (location = 0) in vec3 a_pos;\n"
          "layout (location = 1) in vec3 a_translation;\n"
          "layout (location = 2) in vec2 a_uv;\n"
          "out vec2 v_uv;\n"
          "void main() {\n"
          "  gl_Position = vec4(a_pos+a_translation, 1.0);\n"
          "  v_uv = a_uv;\n"
          "}\n"),
      STR("#version 330 core\n"
          "in vec2 v_uv;\n"
          "out vec4 f_color;\n"
          "uniform sampler2D u_texture;\n"
          "void main() {\n"
          "  f_color = texture(u_texture, v_uv);\n"
          "}\n"),
      ErrPanic);

  sandbox->texture = cm_texture_from_file(
      STR("sandbox/res/textures/claymore-sword.png"), ErrPanic);

  cm_window_bg_color((vec3){0.15f, 0.15f, 0.15f});
}

static void update(CmScene *scene, double deltatime) {
  (void)deltatime;
  Sandbox *sandbox = scene->data;

  cm_texture_bind(&sandbox->texture, 0);

  cm_shader_bind(&sandbox->shader);
  cm_mesh_draw_indexed(&sandbox->mesh, CM_DRAW_TRIANGLES);
}

static void final(CmScene *scene) {
  Sandbox *sandbox = scene->data;
  cm_shader_delete(&sandbox->shader);
  cm_texture_delete(&sandbox->texture);
}

static CmSceneInterface *scene_init(void) {
  static CmSceneInterface sandbox = {
      .init = init,
      .update = update,
      .final = final,
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
      .scene = scene_init,
  };
  return &config;
}
