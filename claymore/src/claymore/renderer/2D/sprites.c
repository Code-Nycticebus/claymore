#include "sprites.h"

#include "claymore/renderer/gpu.h"
#include "claymore/renderer/shaders.h"

#include <stdlib.h>

#include <GL/glew.h>

#define CM_SPRITES_MAX 1000

#define CM_SPRITES_VERTICES 4
#define CM_SPRITES_VERTICES_MAX (CM_SPRITES_MAX * CM_SPRITES_VERTICES)
#define CM_SPRITES_INDICES 6
#define CM_SPRITES_INDICES_MAX (CM_SPRITES_MAX * CM_SPRITES_INDICES)

#define CM_TEXTURE_SLOTS 8

typedef struct {
  vec2 pos;
  vec2 uv;
  float idx;
} Vertex;

struct RenderSpriteData {
  Arena arena;
  CmGpu gpu;

  CmShader shader;

  CmCamera2D *camera;
  usize texture_idx;
  CmTexture2D *texture[CM_TEXTURE_SLOTS];

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
  cm_shader_bind(&renderer->shader);
  cm_shader_set_mat4(&renderer->shader, STR("u_mvp"),
                     renderer->camera->base.vp);

  for (usize i = 0; i < renderer->texture_idx; i++) {
    cm_texture_bind(renderer->texture[i], i);
  }

  cm_gpu_vbo_update(&renderer->vbo, sizeof(Vertex), renderer->vertices_count,
                    (float *)renderer->data);

  cm_gpu_vao_bind(&renderer->vao);

  cm_gpu_ebo_draw(&renderer->ebo, renderer->indices_count, CM_DRAW_TRIANGLES);

  renderer->indices_count = 0;
  renderer->vertices_count = 0;

  renderer->texture_idx = 0;
}

static usize _cm_sprite_push_texture(CmTexture2D *texture) {
  if (CM_TEXTURE_SLOTS <= renderer->texture_idx) {
    cm_sprite_flush();
  }

  for (usize i = 0; i < renderer->texture_idx; i++) {
    if (texture == renderer->texture[i]) {
      return i;
    }
  }
  renderer->texture[renderer->texture_idx++] = texture;
  return renderer->texture_idx - 1;
}

void cm_sprite_push(CmTexture2D *texture, const vec2 position, const vec2 size,
                    float rotation, const vec2 uv, const vec2 uv_size) {
  cebus_assert_debug(renderer, "Renderer 2D was not initialized!");

  usize idx = _cm_sprite_push_texture(texture);

  if (!(renderer->vertices_count < CM_SPRITES_VERTICES_MAX)) {
    cm_sprite_flush();
  }

  // For safety i still assert
  cebus_assert_debug(renderer->vertices_count < CM_SPRITES_VERTICES_MAX, "");
  cebus_assert_debug(renderer->indices_count < CM_SPRITES_INDICES_MAX, "");

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

    vertices[i].idx = idx;
  }

  renderer->vertices_count += CM_SPRITES_VERTICES;
  renderer->indices_count += CM_SPRITES_INDICES;
}

void cm_sprite_internal_begin(CmCamera2D *camera) { renderer->camera = camera; }
void cm_sprite_internal_end(void) {
  if (renderer->vertices_count) {
    cm_sprite_flush();
  }
}

usize cm_sprite_internal_init(void) {
  renderer = calloc(1, sizeof(struct RenderSpriteData));
  renderer->gpu = cm_gpu_internal_init(&renderer->arena);

  renderer->vbo = cm_gpu_vbo(&renderer->gpu, CM_DYNAMIC_DRAW, sizeof(Vertex),
                             CM_SPRITES_VERTICES_MAX, (float *)&renderer->data);

  renderer->vao = cm_gpu_vao(&renderer->gpu);
  cm_gpu_vao_push(&renderer->vao, 2, sizeof(Vertex), offsetof(Vertex, pos));
  cm_gpu_vao_push(&renderer->vao, 2, sizeof(Vertex), offsetof(Vertex, uv));
  cm_gpu_vao_push(&renderer->vao, 1, sizeof(Vertex), offsetof(Vertex, idx));

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
          "layout (location = 2) in float a_idx;\n"
          "uniform mat4 u_mvp;\n"
          "out vec2 v_uv;\n"
          "flat out int v_idx;\n"
          "void main() {\n"
          "  gl_Position = u_mvp * vec4(a_pos.xy, 0.0, 1.0);\n"
          "  v_uv = a_uv;\n"
          "  v_idx = int(a_idx);\n"
          "}\n"),
      STR("#version 330 core\n"
          "in vec2 v_uv;\n"
          "out vec4 f_color;\n"
          "flat in int v_idx;\n"
          "uniform sampler2D u_sampler[8];\n"
          "void main() {\n"
          "  switch (v_idx) {\n"
          "    case 0:\n"
          "      f_color = texture(u_sampler[0], v_uv);\n"
          "      break;\n"
          "    case 1:\n"
          "      f_color = texture(u_sampler[1], v_uv);\n"
          "      break;\n"
          "    case 2:\n"
          "      f_color = texture(u_sampler[2], v_uv);\n"
          "      break;\n"
          "    case 3:\n"
          "      f_color = texture(u_sampler[3], v_uv);\n"
          "      break;\n"
          "    case 4:\n"
          "      f_color = texture(u_sampler[4], v_uv);\n"
          "      break;\n"
          "    case 5:\n"
          "      f_color = texture(u_sampler[5], v_uv);\n"
          "      break;\n"
          "    case 6:\n"
          "      f_color = texture(u_sampler[6], v_uv);\n"
          "      break;\n"
          "    case 7:\n"
          "      f_color = texture(u_sampler[7], v_uv);\n"
          "      break;\n"
          "  }\n"
          "}\n"),
      ErrPanic);

  cm_shader_bind(&renderer->shader);
  u32 loc = glGetUniformLocation(renderer->shader.id, "u_sampler");
  const int slots[8] = {0, 1, 2, 3, 4, 5, 6, 7};
  glUniform1iv(loc, ARRAY_LEN(slots), slots);

  return sizeof(struct RenderSpriteData);
}

void cm_sprite_internal_free(void) {
  cm_gpu_internal_free(&renderer->gpu);
  arena_free(&renderer->arena);
  free(renderer);
}
