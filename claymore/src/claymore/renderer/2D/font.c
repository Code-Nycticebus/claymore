#include "renderer2D.h"

#include "claymore/renderer/shaders.h"

#include <glad.h>
#include <stb_truetype.h>
#include <stdlib.h>

typedef struct {
  vec2 pos;
  vec2 uv;
} Vertex;

#define FONT_CHAR_MIN 32
#define FONT_CHAR_MAX 96

#define FONT_RENDERER_CHAR_MAX 1000
#define FONT_RENDERER_VERTECIES_PER_CHAR 6
#define FONT_RENDERER_VERTECIES_MAX                                            \
  (FONT_RENDERER_CHAR_MAX * FONT_RENDERER_VERTECIES_PER_CHAR)

struct FontRendererData {
  Arena arena;
  CmGpu gpu;

  CmVbo vbo;
  CmVao vao;

  CmShader shader;

  CmCamera2D *camera;
  CmGpuID texture_id;

  Vertex buffer[FONT_RENDERER_VERTECIES_MAX];
  size_t vertex_count;
};

static struct FontRendererData *renderer;

struct CmFont {
  u32 texture_id;
  stbtt_bakedchar cdata[FONT_CHAR_MAX];
  float height;
  size_t ttf_resoulution;
};

CmFont *cm_font_init(CmGpu *gpu, Str filename, float font_height,
                     Error *error) {
  CmFont *font_renderer = arena_calloc(gpu->arena, sizeof(CmFont));

  GLint max_texture_size;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);

  const size_t ttf_bitmap_resolution =
      glm_min(512 * ceilf(font_height / 100.F), max_texture_size);
  const size_t ttf_bitmap_size = ttf_bitmap_resolution * ttf_bitmap_resolution;

  Arena temp = {0};

  Bytes ttf_buffer = file_read_bytes(filename, &temp, error);

  uint8_t *ttf_bitmap = arena_calloc(&temp, sizeof(uint8_t) * ttf_bitmap_size);
  stbtt_BakeFontBitmap(ttf_buffer.data, 0, font_height, ttf_bitmap,
                       ttf_bitmap_resolution, ttf_bitmap_resolution,
                       FONT_CHAR_MIN, FONT_CHAR_MAX, font_renderer->cdata);

  font_renderer->texture_id = cm_gpu_texture(gpu);
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, font_renderer->texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ttf_bitmap_resolution,
               ttf_bitmap_resolution, 0, GL_RED, GL_UNSIGNED_BYTE, ttf_bitmap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  arena_free(&temp);

  font_renderer->height = font_height;
  font_renderer->ttf_resoulution = ttf_bitmap_resolution;

  return font_renderer;
}

static void _cm_font_renderer_flush(void) {
  cm_shader_bind(&renderer->shader);
  cm_shader_set_mat4(&renderer->shader, STR("u_mvp"),
                     renderer->camera->base.vp);
  cm_shader_set_i32(&renderer->shader, STR("u_texture"), 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, renderer->texture_id);

  glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo.id);
  glBindVertexArray(renderer->vao.id);

  cm_gpu_vbo_update(&renderer->vbo, sizeof(Vertex), renderer->vertex_count,
                    (float *)renderer->buffer);
  glDisable(GL_DEPTH_TEST);
  glDrawArrays(GL_TRIANGLES, 0, renderer->vertex_count);
  glEnable(GL_DEPTH_TEST);
  renderer->vertex_count = 0;
}

void cm_font(CmFont *font, const vec2 pos, Str text) {
  renderer->texture_id = font->texture_id;

  float text_y = pos[1] + font->height;
  float text_x = pos[0];
  Vertex *vertex = renderer->buffer;
  for (size_t i = 0; i < text.len; i++) {
    if (FONT_CHAR_MIN <= text.data[i] &&
        text.data[i] < FONT_CHAR_MIN + FONT_CHAR_MAX - 1) {

      if (!(renderer->vertex_count < FONT_RENDERER_VERTECIES_MAX)) {
        _cm_font_renderer_flush();
        vertex = renderer->buffer;
      }

      stbtt_aligned_quad q;
      stbtt_GetBakedQuad(font->cdata, font->ttf_resoulution,
                         font->ttf_resoulution, text.data[i] - FONT_CHAR_MIN,
                         &text_x, &text_y, &q, 1);

      glm_vec2_copy((vec2){q.x0, q.y0}, vertex[0].pos);
      glm_vec2_copy((vec2){q.s0, q.t0}, vertex[0].uv);

      glm_vec2_copy((vec2){q.x1, q.y0}, vertex[1].pos);
      glm_vec2_copy((vec2){q.s1, q.t0}, vertex[1].uv);

      glm_vec2_copy((vec2){q.x1, q.y1}, vertex[2].pos);
      glm_vec2_copy((vec2){q.s1, q.t1}, vertex[2].uv);

      glm_vec2_copy((vec2){q.x0, q.y1}, vertex[3].pos);
      glm_vec2_copy((vec2){q.s0, q.t1}, vertex[3].uv);

      glm_vec2_copy((vec2){q.x0, q.y0}, vertex[4].pos);
      glm_vec2_copy((vec2){q.s0, q.t0}, vertex[4].uv);

      glm_vec2_copy((vec2){q.x1, q.y1}, vertex[5].pos); // NOLINT
      glm_vec2_copy((vec2){q.s1, q.t1}, vertex[5].uv);  // NOLINT

      renderer->vertex_count += FONT_RENDERER_VERTECIES_PER_CHAR;
      vertex += FONT_RENDERER_VERTECIES_PER_CHAR;
    } else if (text.data[i] == '\n') {
      text_y += font->height;
      text_x = pos[0];
    }
  }
}

void cm_font_internal_begin(CmCamera2D *camera) { renderer->camera = camera; }

void cm_font_internal_end(void) {
  if (renderer->vertex_count) {
    _cm_font_renderer_flush();
  }
}

usize cm_font_internal_init(void) {
  renderer = calloc(1, sizeof(struct FontRendererData));

  renderer->gpu = cm_gpu_internal_init(&renderer->arena);

  renderer->shader = cm_shader_from_memory(
      &renderer->gpu,
      STR("#version 120\n"
          "attribute vec2 a_pos;\n"
          "attribute vec2 a_uv;\n"
          "varying vec2 v_uv;\n"
          "uniform mat4 u_mvp;\n"
          "void main() {\n"
          "  gl_Position = u_mvp * vec4(a_pos.xy, 0, 1.0);\n"
          "  v_uv = a_uv;\n"
          "}\n"),
      STR("#version 120\n"
          "varying vec2 v_uv;\n"
          "uniform sampler2D u_texture;\n"
          "void main() {\n"
          " gl_FragColor = vec4(texture2D(u_texture, v_uv).r);\n"
          "}\n"),
      ErrDefault);

  renderer->vbo = cm_gpu_vbo(
      &renderer->gpu, CM_DYNAMIC_DRAW, sizeof(Vertex),
      FONT_RENDERER_VERTECIES_PER_CHAR * FONT_RENDERER_CHAR_MAX, NULL);

  renderer->vao = cm_gpu_vao(&renderer->gpu);
  cm_gpu_vao_push(&renderer->vao, 2, sizeof(Vertex), offsetof(Vertex, pos));
  cm_gpu_vao_push(&renderer->vao, 2, sizeof(Vertex), offsetof(Vertex, uv));

  return sizeof(struct FontRendererData);
}

void cm_font_internal_free(void) {
  cm_gpu_internal_free(&renderer->gpu);
  arena_free(&renderer->arena);
  free(renderer);
}
