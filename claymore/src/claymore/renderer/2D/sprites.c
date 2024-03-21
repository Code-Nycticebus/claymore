#include "sprites.h"

#include "claymore/renderer/gpu.h"
#include "claymore/renderer/shaders.h"

#include <stdlib.h>

#define CM_SPRITES_MAX 1000

#define CM_SPRITES_VERTICES 4
#define CM_SPRITES_VERTICES_MAX (CM_SPRITES_MAX * CM_SPRITES_VERTICES)
#define CM_SPRITES_INDICES 6
#define CM_SPRITES_INDICES_MAX (CM_SPRITES_MAX * CM_SPRITES_INDICES)

typedef struct {
  vec2 pos;
  vec2 uv;
} Vertex;

struct RenderSpriteData {
  Arena arena;
  CmGpu gpu;

  CmShader shader;

  CmVbo vbo;
  CmVao vao;
  CmEbo ebo;

  size_t vertices_count;
  Vertex data[CM_SPRITES_VERTICES_MAX];

  size_t indices_count;
  uint32_t indices[CM_SPRITES_INDICES_MAX];
};

static struct RenderSpriteData *renderer = NULL;

static void cm_sprite_flush(void) {
  cm_gpu_vbo_update(&renderer->vbo, sizeof(Vertex), renderer->vertices_count,
                    (float *)renderer->data);

  cm_gpu_vao_bind(&renderer->vao);

  cm_gpu_ebo_draw(&renderer->ebo, renderer->indices_count, CM_DRAW_TRIANGLES);

  renderer->indices_count = 0;
  renderer->vertices_count = 0;
}

void cm_sprite_begin(mat4 mvp, CmTexture2D *texture) {
  cm_shader_bind(&renderer->shader);
  cm_shader_set_mat4(&renderer->shader, STR("u_mvp"), mvp);
  cm_shader_set_i32(&renderer->shader, STR("u_sampler"), 0);
  cm_texture_bind(texture, 0);
}
void cm_sprite_end(void) { cm_sprite_flush(); }

void cm_sprite_push(const vec2 position, const vec2 size, float rotation,
                    const vec2 uv, const vec2 uv_size) {
  clib_assert_debug(renderer, "Renderer 2D was not initialized!");
  if (!(renderer->vertices_count < CM_SPRITES_VERTICES_MAX)) {
    cm_sprite_flush();
  }

  // For safety i still assert
  clib_assert_debug(renderer->vertices_count < CM_SPRITES_VERTICES_MAX, "");
  clib_assert_debug(renderer->indices_count < CM_SPRITES_INDICES_MAX, "");

  float cos_theta;
  float sin_theta;
  if (rotation != 0) {
    cos_theta = cosf(rotation);
    sin_theta = sinf(rotation);
  }

  struct {
    vec2 size;
    vec2 uv;
  } sprite[CM_SPRITES_VERTICES] = {
      {{0, 0}, {uv[0], uv[1]}},
      {{size[0], 0}, {uv[0] + uv_size[0], uv[1]}},
      {{size[0], size[1]}, {uv[0] + uv_size[0], uv[1] + uv_size[1]}},
      {{0, size[1]}, {uv[0], uv[1] + uv_size[1]}},
  };
  Vertex *vertices = &renderer->data[renderer->vertices_count];
  for (int i = 0; i < CM_SPRITES_VERTICES; ++i) {
    if (rotation != 0.f) {
      const float x = sprite[i].size[0];
      const float y = sprite[i].size[1];
      sprite[i].size[0] = x * cos_theta - y * sin_theta;
      sprite[i].size[1] = x * sin_theta + y * cos_theta;
    }

    vertices[i].pos[0] = sprite[i].size[0] + position[0];
    vertices[i].pos[1] = sprite[i].size[1] + position[1];

    vertices[i].uv[0] = sprite[i].uv[0];
    vertices[i].uv[1] = sprite[i].uv[1];
  }

  renderer->vertices_count += CM_SPRITES_VERTICES;
  renderer->indices_count += CM_SPRITES_INDICES;
}

void cm_sprite_internal_init(void) {
  renderer = calloc(1, sizeof(struct RenderSpriteData));
  renderer->gpu = cm_gpu_internal_init(&renderer->arena);

  renderer->vbo = cm_gpu_vbo(&renderer->gpu, CM_DYNAMIC_DRAW, sizeof(Vertex),
                             CM_SPRITES_VERTICES_MAX, (float *)&renderer->data);

  renderer->vao = cm_gpu_vao(&renderer->gpu);
  cm_gpu_vao_push(&renderer->vao, 2, sizeof(Vertex), offsetof(Vertex, pos));
  cm_gpu_vao_push(&renderer->vao, 2, sizeof(Vertex), offsetof(Vertex, uv));

  u32 indices[CM_SPRITES_INDICES] = {0, 1, 3, 1, 2, 3};
  for (size_t i = 0; i < CM_SPRITES_INDICES_MAX; ++i) {
    renderer->indices[i] = indices[i % CM_SPRITES_INDICES];
    renderer->indices[i] += (CM_SPRITES_VERTICES * (i / CM_SPRITES_INDICES));
  }
  renderer->ebo = cm_gpu_ebo(&renderer->gpu, CM_STATIC_DRAW,
                             CM_SPRITES_INDICES_MAX, renderer->indices);

  renderer->shader = cm_shader_from_memory(
      &renderer->gpu,
      STR("#version 330 core\n"
          "layout (location = 0) in vec2 a_pos;\n"
          "layout (location = 1) in vec2 a_uv;\n"
          "uniform mat4 u_mvp;\n"
          "out vec2 v_uv;\n"
          "void main() {\n"
          "  gl_Position = u_mvp * vec4(a_pos.xy, 0.0, 1.0);\n"
          "  v_uv = a_uv;\n"
          "}\n"),
      STR("#version 330 core\n"
          "in vec2 v_uv;\n"
          "out vec4 f_color;\n"
          "uniform sampler2D u_sampler;\n"
          "void main() {\n"
          "  f_color = texture(u_sampler, v_uv);\n"
          "}\n"),
      ErrPanic);

  const usize bytes = sizeof(struct RenderSpriteData) / 1000;
  clib_log_info("Sprite Renderer: %" USIZE_FMT " kb", bytes);
}

void cm_sprite_internal_free(void) {
  cm_gpu_internal_free(&renderer->gpu);
  arena_free(&renderer->arena);
  free(renderer);
}
