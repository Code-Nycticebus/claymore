#include "font.h"

#include "claymore/core/app.h"
#include "claymore/renderer/renderer.h"
#include "claymore/renderer/shader.h"

#include "stb_truetype.h"

#include <errno.h>
#include <string.h>

static const char font_vs[] = //
    "#version 120\n"
    "attribute vec3 a_pos;\n"
    "attribute vec2 a_uv;\n"
    "varying vec2 v_uv;\n"
    "uniform mat4 u_mvp;\n"
    "void main() {\n"
    "  gl_Position = u_mvp * vec4(a_pos.xyz, 1.0);\n"
    "  v_uv = a_uv;\n"
    "}\n";

static const char font_fs[] = //
    "#version 120\n"
    "varying vec2 v_uv;\n"
    "uniform sampler2D u_texture;\n"
    "void main() {\n"
    " gl_FragColor = vec4(texture2D(u_texture, v_uv).r);\n"
    "}\n";

struct Vertex {
  vec3 pos;
  vec2 uv;
};

#define FONT_CHAR_MIN 32
#define FONT_CHAR_MAX 96

#define FONT_RENDERER_CHAR_MAX 64
#define FONT_RENDERER_VERTECIES_PER_CHAR 6
#define FONT_RENDERER_VERTECIES_MAX                                            \
  (FONT_RENDERER_CHAR_MAX * FONT_RENDERER_VERTECIES_PER_CHAR)

struct CmFont {
  CmShader shader;
  GLuint texture_id;
  CmVertexBuffer vertex_buffer;
  CmVertexAttribute vertex_attrib;

  stbtt_bakedchar cdata[FONT_CHAR_MAX];

  struct Vertex buffer[FONT_RENDERER_VERTECIES_MAX];
  size_t vertex_count;

  float height;
  size_t ttf_resoulution;
};

CmFont *cm_font_init(const char *filename, float font_height) {
  CmFont *font_renderer = (CmFont *)calloc(sizeof(CmFont), 1);
  uint8_t *ttf_buffer;

  font_renderer->shader = cm_shader_load_from_memory(font_vs, font_fs);

  GLint max_texture_size;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);

  const size_t ttf_bitmap_resolution =
      glm_min(512 * ceilf(font_height / 100.F), max_texture_size);
  const size_t ttf_bitmap_size = ttf_bitmap_resolution * ttf_bitmap_resolution;
  uint8_t *ttf_bitmap = (uint8_t *)calloc(1, sizeof(uint8_t) * ttf_bitmap_size);

  FILE *ttf_file = fopen(filename, "rb");
  if (ttf_file == NULL) {
    cm_log_error("Could not open %s: %s\n", filename, strerror(errno));
    return NULL;
  }

  fseek(ttf_file, 0, SEEK_END);
  const size_t buffer_size = ftell(ttf_file);
  fseek(ttf_file, 0, SEEK_SET);

  ttf_buffer = (uint8_t *)malloc(buffer_size * sizeof(uint8_t));

  fread(ttf_buffer, 1, buffer_size, ttf_file);

  stbtt_BakeFontBitmap(ttf_buffer, 0, font_height, ttf_bitmap,
                       ttf_bitmap_resolution, ttf_bitmap_resolution,
                       FONT_CHAR_MIN, FONT_CHAR_MAX, font_renderer->cdata);

  glGenTextures(1, &font_renderer->texture_id);
  glBindTexture(GL_TEXTURE_2D, font_renderer->texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ttf_bitmap_resolution,
               ttf_bitmap_resolution, 0, GL_RED, GL_UNSIGNED_BYTE, ttf_bitmap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  free(ttf_bitmap);
  free(ttf_buffer);
  fclose(ttf_file);

  font_renderer->vertex_buffer = cm_vertex_buffer_create(
      FONT_RENDERER_VERTECIES_PER_CHAR * FONT_RENDERER_CHAR_MAX,
      sizeof(struct Vertex), 0, GL_DYNAMIC_DRAW);

  font_renderer->vertex_attrib =
      cm_vertex_attribute_create(&font_renderer->vertex_buffer);
  cm_vertex_attribute_push(&font_renderer->vertex_attrib, 3, GL_FLOAT,
                           offsetof(struct Vertex, pos));
  cm_vertex_attribute_push(&font_renderer->vertex_attrib, 2, GL_FLOAT,
                           offsetof(struct Vertex, uv));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  font_renderer->height = font_height;
  font_renderer->ttf_resoulution = ttf_bitmap_resolution;

  return font_renderer;
}

void _cm_font_renderer_flush(CmFont *font) {
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  sizeof(struct Vertex) * font->vertex_count, font->buffer);
  glDrawArrays(GL_TRIANGLES, 0, font->vertex_count);
  font->vertex_count = 0;
}

void cm_font_draw(CmFont *font, const mat4s mvp, float x, float y, float z,
                  size_t len, const char *text) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, font->texture_id);

  mat4s inverted_mvp;
  inverted_mvp = glms_scale(mvp, (vec3s){{1.0F, -1.0F, 1.0F}});

  cm_shader_bind(&font->shader);
  cm_shader_set_mat4(&font->shader, "u_mvp", inverted_mvp);
  cm_shader_set_i32(&font->shader, "u_texture", 0);

  glBindBuffer(GL_ARRAY_BUFFER, font->vertex_buffer.id);
  glBindVertexArray(font->vertex_attrib.id);

  float inverted_y = -y;
  struct Vertex *current_vertex = font->buffer;
  for (size_t i = 0; i < len; i++) {
    if (FONT_CHAR_MIN <= text[i] &&
        text[i] < FONT_CHAR_MIN + FONT_CHAR_MAX - 1) {

      if (!(font->vertex_count < FONT_RENDERER_VERTECIES_MAX)) {
        _cm_font_renderer_flush(font);
        current_vertex = font->buffer;
      }

      stbtt_aligned_quad q;
      stbtt_GetBakedQuad(font->cdata, font->ttf_resoulution,
                         font->ttf_resoulution, text[i] - FONT_CHAR_MIN, &x,
                         &inverted_y, &q, 1);

      glm_vec3_copy((vec3){q.x0, q.y0, z}, current_vertex[0].pos);
      glm_vec2_copy((vec2){q.s0, q.t0}, current_vertex[0].uv);

      glm_vec3_copy((vec3){q.x1, q.y0, z}, current_vertex[1].pos);
      glm_vec2_copy((vec2){q.s1, q.t0}, current_vertex[1].uv);

      glm_vec3_copy((vec3){q.x1, q.y1, z}, current_vertex[2].pos);
      glm_vec2_copy((vec2){q.s1, q.t1}, current_vertex[2].uv);

      glm_vec3_copy((vec3){q.x0, q.y1, z}, current_vertex[3].pos);
      glm_vec2_copy((vec2){q.s0, q.t1}, current_vertex[3].uv);

      glm_vec3_copy((vec3){q.x0, q.y0, z}, current_vertex[4].pos);
      glm_vec2_copy((vec2){q.s0, q.t0}, current_vertex[4].uv);

      glm_vec3_copy((vec3){q.x1, q.y1, z}, current_vertex[4 + 1].pos);
      glm_vec2_copy((vec2){q.s1, q.t1}, current_vertex[4 + 1].uv);

      font->vertex_count += FONT_RENDERER_VERTECIES_PER_CHAR;
      current_vertex += FONT_RENDERER_VERTECIES_PER_CHAR;
    }
  }

  _cm_font_renderer_flush(font);
  glBindTexture(GL_TEXTURE_2D, 0);
  cm_shader_unbind();
}

void cm_font_draw_cstr(CmFont *font, const mat4s mvp, float x, float y, float z,
                       const char *text) {
  size_t len = strlen(text);
  cm_font_draw(font, mvp, x, y, z, len, text);
}

void cm_font_free(CmFont *font) {
  glDeleteTextures(1, &font->texture_id);
  glDeleteBuffers(1, &font->vertex_buffer.id);
  glDeleteVertexArrays(1, &font->vertex_attrib.id);
  cm_shader_delete(&font->shader);
  free(font);
}