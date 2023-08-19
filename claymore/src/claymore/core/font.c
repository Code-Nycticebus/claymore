#include "font.h"

#include "claymore/core/app.h"
#include "claymore/core/shader.h"
#include "claymore/renderer/renderer.h"

#include "stb_truetype.h"

#include <errno.h>
#include <string.h>

typedef struct {
  uint32_t id;

  struct {
    GLint mvp;
    GLint texture;
  } uniform_loc;
} FontShader;

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

#define FONT_HEIGHT 32.F
#define FONT_CHAR_MIN 32
#define FONT_CHAR_MAX 96

#define FONT_RENDERER_CHAR_MAX 20
#define FONT_RENDERER_VERTECIES_PER_CHAR 6
#define FONT_RENDERER_VERTECIES_MAX                                            \
  FONT_RENDERER_CHAR_MAX *FONT_RENDERER_VERTECIES_PER_CHAR

static stbtt_bakedchar cdata[FONT_CHAR_MAX];

typedef struct {
  FontShader shader;
  GLuint texture_id;
  CmVertexBuffer vertex_buffer;
  CmVertexAttribute vertex_attrib;

  struct Vertex buffer[FONT_RENDERER_VERTECIES_MAX];
  size_t vertex_count;
} FontRenderer;
static FontRenderer *font_renderer;

void cm_font_init(const char *filename) {
  assert(font_renderer == NULL && "Fonts initialized twice");
  font_renderer = (FontRenderer *)calloc(sizeof(FontRenderer), 1);
  uint8_t *ttf_buffer;
  font_renderer->shader.id = cm_load_shader_from_memory(font_vs, font_fs);
  font_renderer->shader.uniform_loc.mvp =
      cm_shader_get_uniform_location(font_renderer->shader.id, "u_mvp");
  font_renderer->shader.uniform_loc.texture =
      cm_shader_get_uniform_location(font_renderer->shader.id, "u_texture");

#define TTF_BUFFER_MAX (1 << 20)
  ttf_buffer = (uint8_t *)malloc(TTF_BUFFER_MAX * sizeof(uint8_t));

#define TTF_BITMAP_RESOLUTION 512
#define TTF_BITMAP_MAX (TTF_BITMAP_RESOLUTION * TTF_BITMAP_RESOLUTION)
  uint8_t ttf_bitmap[TTF_BITMAP_MAX];

  FILE *ttf_file = fopen(filename, "rb");
  if (ttf_file == NULL) {
    cm_log_error("Could not open %s: %s", filename, strerror(errno));
  }

  fread(ttf_buffer, 1, TTF_BUFFER_MAX, ttf_file);

  stbtt_BakeFontBitmap(ttf_buffer, 0, FONT_HEIGHT, ttf_bitmap,
                       TTF_BITMAP_RESOLUTION, TTF_BITMAP_RESOLUTION,
                       FONT_CHAR_MIN, FONT_CHAR_MAX, cdata);

  fclose(ttf_file);

  glGenTextures(1, &font_renderer->texture_id);
  glBindTexture(GL_TEXTURE_2D, font_renderer->texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, TTF_BITMAP_RESOLUTION,
               TTF_BITMAP_RESOLUTION, 0, GL_RED, GL_UNSIGNED_BYTE, ttf_bitmap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

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

  free(ttf_buffer);
}

void cm_font_draw(mat4 mvp, float x, float y, size_t len, const char *text) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, font_renderer->texture_id);

  glUseProgram(font_renderer->shader.id);
  glUniformMatrix4fv(font_renderer->shader.uniform_loc.mvp, 1, GL_FALSE,
                     (float *)mvp);
  glUniform1i(font_renderer->shader.uniform_loc.texture, 0);
  glBindBuffer(GL_ARRAY_BUFFER, font_renderer->vertex_buffer.id);
  glBindVertexArray(font_renderer->vertex_attrib.id);

  struct Vertex *current_vertex = font_renderer->buffer;
  static bool start = false;
  for (size_t i = 0; i < len; i++) {
    if (FONT_CHAR_MIN <= text[i] &&
        text[i] < FONT_CHAR_MIN + FONT_CHAR_MAX - 1) {
      stbtt_aligned_quad q;
      stbtt_GetBakedQuad(cdata, TTF_BITMAP_RESOLUTION, TTF_BITMAP_RESOLUTION,
                         text[i] - FONT_CHAR_MIN, &x, &y, &q, 1);

      glm_vec3_copy((vec3){q.x0, q.y0, 0.0F}, current_vertex[0].pos);

      glm_vec2_copy((vec2){q.s0, q.t0}, current_vertex[0].uv);

      glm_vec3_copy((vec3){q.x1, q.y0, 0.0F}, current_vertex[1].pos);
      glm_vec2_copy((vec2){q.s1, q.t0}, current_vertex[1].uv);

      glm_vec3_copy((vec3){q.x1, q.y1, 0.0F}, current_vertex[2].pos);
      glm_vec2_copy((vec2){q.s1, q.t1}, current_vertex[2].uv);

      glm_vec3_copy((vec3){q.x0, q.y1, 0.0F}, current_vertex[3].pos);
      glm_vec2_copy((vec2){q.s0, q.t1}, current_vertex[3].uv);

      glm_vec3_copy((vec3){q.x0, q.y0, 0.0F}, current_vertex[4].pos);
      glm_vec2_copy((vec2){q.s0, q.t0}, current_vertex[4].uv);

      glm_vec3_copy((vec3){q.x1, q.y1, 0.0F}, current_vertex[4 + 1].pos);
      glm_vec2_copy((vec2){q.s1, q.t1}, current_vertex[4 + 1].uv);

      if (start) {
        cm_log_trace("%f %f %f\n", current_vertex[0].pos[0],
                     current_vertex[0].pos[1], current_vertex[0].pos[2]);
        start = true;
      }

      font_renderer->vertex_count += FONT_RENDERER_VERTECIES_PER_CHAR;
      current_vertex += FONT_RENDERER_VERTECIES_PER_CHAR;

      assert(font_renderer->vertex_count < FONT_RENDERER_VERTECIES_MAX);
    }
  }

  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  sizeof(struct Vertex) * font_renderer->vertex_count,
                  font_renderer->buffer);

  glDrawArrays(GL_TRIANGLES, 0, font_renderer->vertex_count);

  font_renderer->vertex_count = 0;

  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
}

void cm_font_free(void) {}
