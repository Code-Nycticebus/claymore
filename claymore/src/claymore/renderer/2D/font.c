#include "font.h"

#include "shaders.h"

#include <GL/glew.h>
#include <stb_truetype.h>

typedef struct {
  vec2 pos;
  vec2 uv;
} Vertex;

#define FONT_CHAR_MIN 32
#define FONT_CHAR_MAX 96

#define FONT_RENDERER_CHAR_MAX 64
#define FONT_RENDERER_VERTECIES_PER_CHAR 6
#define FONT_RENDERER_VERTECIES_MAX                                            \
  (FONT_RENDERER_CHAR_MAX * FONT_RENDERER_VERTECIES_PER_CHAR)

struct CmFont {
  CmShader shader;
  u32 texture_id;
  CmVbo vertex_buffer;
  CmVao vertex_array;

  stbtt_bakedchar cdata[FONT_CHAR_MAX];

  Vertex buffer[FONT_RENDERER_VERTECIES_MAX];
  size_t vertex_count;

  float height;
  size_t ttf_resoulution;
};

CmFont *cm_font_init(CmGpu *gpu, Str filename, float font_height,
                     Error *error) {
  CmFont *font_renderer = arena_calloc(gpu->arena, sizeof(CmFont));

  font_renderer->shader = cm_shader_from_memory(
      gpu,
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

  glGenTextures(1, &font_renderer->texture_id);
  glBindTexture(GL_TEXTURE_2D, font_renderer->texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ttf_bitmap_resolution,
               ttf_bitmap_resolution, 0, GL_RED, GL_UNSIGNED_BYTE, ttf_bitmap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  arena_free(&temp);

  font_renderer->vertex_buffer = cm_gpu_vbo(
      gpu, CM_DYNAMIC_DRAW, sizeof(Vertex),
      FONT_RENDERER_VERTECIES_PER_CHAR * FONT_RENDERER_CHAR_MAX, NULL);

  font_renderer->vertex_array = cm_gpu_vao(gpu);
  cm_gpu_vao_push(&font_renderer->vertex_array, 2, sizeof(Vertex),
                  offsetof(Vertex, pos));
  cm_gpu_vao_push(&font_renderer->vertex_array, 2, sizeof(Vertex),
                  offsetof(Vertex, uv));

  font_renderer->height = font_height;
  font_renderer->ttf_resoulution = ttf_bitmap_resolution;

  return font_renderer;
}

static void _cm_font_renderer_flush(CmFont *font) {
  cm_gpu_vbo_update(&font->vertex_buffer, sizeof(Vertex), font->vertex_count,
                    (float *)font->buffer);
  glDrawArrays(GL_TRIANGLES, 0, font->vertex_count);
  font->vertex_count = 0;
}

void cm_font_draw(CmFont *font, const mat4 mvp, const vec2 pos, Str text) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, font->texture_id);

  cm_shader_bind(&font->shader);
  cm_shader_set_mat4(&font->shader, STR("u_mvp"), mvp);
  cm_shader_set_i32(&font->shader, STR("u_texture"), 0);

  glBindBuffer(GL_ARRAY_BUFFER, font->vertex_buffer.id);
  glBindVertexArray(font->vertex_array.id);

  float text_y = pos[1];
  float text_x = pos[0];
  Vertex *current_vertex = font->buffer;
  for (size_t i = 0; i < text.len; i++) {
    if (FONT_CHAR_MIN <= text.data[i] &&
        text.data[i] < FONT_CHAR_MIN + FONT_CHAR_MAX - 1) {

      if (!(font->vertex_count < FONT_RENDERER_VERTECIES_MAX)) {
        _cm_font_renderer_flush(font);
        current_vertex = font->buffer;
      }

      stbtt_aligned_quad q;
      stbtt_GetBakedQuad(font->cdata, font->ttf_resoulution,
                         font->ttf_resoulution, text.data[i] - FONT_CHAR_MIN,
                         &text_x, &text_y, &q, 1);

      glm_vec2_copy((vec2){q.x0, q.y0}, current_vertex[0].pos);
      glm_vec2_copy((vec2){q.s0, q.t0}, current_vertex[0].uv);

      glm_vec2_copy((vec2){q.x1, q.y0}, current_vertex[1].pos);
      glm_vec2_copy((vec2){q.s1, q.t0}, current_vertex[1].uv);

      glm_vec2_copy((vec2){q.x1, q.y1}, current_vertex[2].pos);
      glm_vec2_copy((vec2){q.s1, q.t1}, current_vertex[2].uv);

      glm_vec2_copy((vec2){q.x0, q.y1}, current_vertex[3].pos);
      glm_vec2_copy((vec2){q.s0, q.t1}, current_vertex[3].uv);

      glm_vec2_copy((vec2){q.x0, q.y0}, current_vertex[4].pos);
      glm_vec2_copy((vec2){q.s0, q.t0}, current_vertex[4].uv);

      glm_vec2_copy((vec2){q.x1, q.y1}, current_vertex[5].pos); // NOLINT
      glm_vec2_copy((vec2){q.s1, q.t1}, current_vertex[5].uv);  // NOLINT

      font->vertex_count += FONT_RENDERER_VERTECIES_PER_CHAR;
      current_vertex += FONT_RENDERER_VERTECIES_PER_CHAR;
    } else if (text.data[i] == '\n') {
      text_y += font->height;
      text_x = pos[0];
    }
  }

  _cm_font_renderer_flush(font);
  glBindTexture(GL_TEXTURE_2D, 0);
  cm_shader_unbind();
}
