#include "claymore.h"
#include <stdio.h>

#include <errno.h>
#include <string.h>

#include "stb_truetype.h"

struct ShaderData {
  uint32_t id;

  struct {
    GLint mvp;
    GLint texture;
  } uniform_loc;
};

static struct ShaderData font_shader;

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
  GLuint texture_id;
  CmVertexBuffer vertex_buffer;
  CmVertexAttribute vertex_attrib;

  struct Vertex buffer[FONT_RENDERER_VERTECIES_MAX];
  size_t vertex_count;
} FontRenderer;

static FontRenderer font_renderer;


static uint8_t* ttf_buffer;

static void fps_init(CmLayer *layer) {
  (void)layer;
  font_shader.id = cm_load_shader_from_file("res/shader/font.vs.glsl",
                                            "res/shader/font.fs.glsl");
  font_shader.uniform_loc.mvp =
      cm_shader_get_uniform_location(font_shader.id, "u_mvp");
  font_shader.uniform_loc.texture =
      cm_shader_get_uniform_location(font_shader.id, "u_texture");

  glm_ortho(0.F, (float)layer->app->window->width,
            (float)layer->app->window->height, 0.F, -1.F, 100.F,
            layer->camera.projection);

  glm_mat4_identity(layer->camera.view);
  glm_vec3_copy((vec3){0}, layer->camera.position);
  glm_translate(layer->camera.view, layer->camera.position);
  layer->camera.update = true;

#define TTF_BUFFER_MAX (1 << 20)
  ttf_buffer = (uint8_t*)malloc(TTF_BUFFER_MAX*sizeof(uint8_t));

#define TTF_BITMAP_RESOLUTION 512
#define TTF_BITMAP_MAX (TTF_BITMAP_RESOLUTION * TTF_BITMAP_RESOLUTION)
  uint8_t ttf_bitmap[TTF_BITMAP_MAX];

  const char *filename = "res/fonts/Ubuntu.ttf";
  FILE *ttf_file = fopen(filename, "rb");
  if (ttf_file == NULL) {
    cm_log_error("Could not open %s: %s", filename, strerror(errno));
  }

  fread(ttf_buffer, 1, TTF_BUFFER_MAX, ttf_file);

  stbtt_BakeFontBitmap(ttf_buffer, 0, FONT_HEIGHT, ttf_bitmap,
                       TTF_BITMAP_RESOLUTION, TTF_BITMAP_RESOLUTION,
                       FONT_CHAR_MIN, FONT_CHAR_MAX, cdata);

  fclose(ttf_file);

  glGenTextures(1, &font_renderer.texture_id);
  glBindTexture(GL_TEXTURE_2D, font_renderer.texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, TTF_BITMAP_RESOLUTION,
               TTF_BITMAP_RESOLUTION, 0, GL_RED, GL_UNSIGNED_BYTE, ttf_bitmap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  font_renderer.vertex_buffer = cm_vertex_buffer_create(
      FONT_RENDERER_VERTECIES_PER_CHAR * FONT_RENDERER_CHAR_MAX,
      sizeof(struct Vertex), 0, GL_DYNAMIC_DRAW);

  font_renderer.vertex_attrib =
      cm_vertex_attribute_create(&font_renderer.vertex_buffer);
  cm_vertex_attribute_push(&font_renderer.vertex_attrib, 3, GL_FLOAT,
                           offsetof(struct Vertex, pos));
  cm_vertex_attribute_push(&font_renderer.vertex_attrib, 2, GL_FLOAT,
                           offsetof(struct Vertex, uv));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

static void draw_text(float x, float y, size_t len, const char *text) {
  glBindBuffer(GL_ARRAY_BUFFER, font_renderer.vertex_buffer.id);
  glBindVertexArray(font_renderer.vertex_attrib.id);

  struct Vertex *current_vertex = font_renderer.buffer;
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

      font_renderer.vertex_count += FONT_RENDERER_VERTECIES_PER_CHAR;
      current_vertex += FONT_RENDERER_VERTECIES_PER_CHAR;

      assert(font_renderer.vertex_count < FONT_RENDERER_VERTECIES_MAX);
    }
  }

  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  sizeof(struct Vertex) * font_renderer.vertex_count,
                  font_renderer.buffer);

  glDrawArrays(GL_TRIANGLES, 0, font_renderer.vertex_count);

  font_renderer.vertex_count = 0;

  glBindTexture(GL_TEXTURE_2D, 0);
}

static void fps_update(CmLayer *layer, float dt) {
  (void)layer, (void)dt;
  glDisable(GL_DEPTH_TEST);

  mat4 mvp;
  mat4 model;
  glm_mat4_identity(model);
  glm_mat4_mul(layer->camera.vp, model, mvp);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, font_renderer.texture_id);
  glUseProgram(font_shader.id);
  glUniformMatrix4fv(font_shader.uniform_loc.mvp, 1, GL_FALSE, (float *)mvp);
  glUniform1i(font_shader.uniform_loc.texture, 0);

  const float fps_y = 30.F;
  const float fps_x = 10.F;
#define FPS_MAX 32
  char fps_buffer[FPS_MAX] = {0};
  size_t len = snprintf(fps_buffer, FPS_MAX - 1, "%.0f", 1 / dt);
  draw_text(fps_x, fps_y, len, fps_buffer);

  glEnable(GL_DEPTH_TEST);
}

static void fps_free(CmLayer *layer) { (void)layer;
  free(ttf_buffer);
}

CmLayerInterface sandbox_fps(void) {
  return (CmLayerInterface){
      .init = fps_init,
      .free = fps_free,
      .update = fps_update,
  };
}
