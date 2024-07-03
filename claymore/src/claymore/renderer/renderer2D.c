#include "renderer2D.h"

#include "claymore/assets/shaders.h"
#include "claymore/renderer/gpu.h"

/* ============= type declarations ============= */

#define CM_QUADS_MAX 1000
#define CM_QUADS_VERTICES 4
#define CM_QUADS_VERTICES_MAX (CM_QUADS_MAX * CM_QUADS_VERTICES)
#define CM_QUADS_INDICES 6
#define CM_QUADS_INDICES_MAX (CM_QUADS_MAX * CM_QUADS_INDICES)
typedef struct {
  vec2 pos;
  vec4 color;
} QuadVertex;

#define CM_CIRCLES_MAX 1000
typedef struct {
  vec2 p;
  float r;
  vec4 c;
} CircleVertex;

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
} SpriteVertex;

#define CM_LINES_MAX 1000
#define CM_LINES_VERTICES 2
#define CM_LINES_VERTICES_MAX (CM_LINES_MAX * CM_LINES_VERTICES)
typedef struct {
  vec2 pos;
} LineVertex;

#define FONT_RENDERER_CHAR_MAX 1000
#define FONT_RENDERER_VERTECIES_PER_CHAR 6
#define FONT_RENDERER_VERTECIES_MAX                                            \
  (FONT_RENDERER_CHAR_MAX * FONT_RENDERER_VERTECIES_PER_CHAR)
typedef struct {
  vec2 pos;
  vec2 uv;
} FontVertex;

struct CmRenderer2D {
  CmCamera2D *camera;

  Arena arena;
  CmGpu gpu;

  struct QuadRenderer {
    CmShader shader;

    CmVbo vbo;
    CmVao vao;
    CmEbo ebo;

    size_t vertices_count;
    QuadVertex data[CM_QUADS_VERTICES_MAX];

    size_t indices_count;
    uint32_t indices[CM_QUADS_INDICES_MAX];
  } quad;

  struct CircleRenderer {
    CmShader shader;
    CmVbo vbo;
    CmVao vao;

    usize vertex_count;
    CircleVertex vertices[CM_CIRCLES_MAX];
  } circle;

  struct SpriteRenderer {
    CmShader shader;

    usize texture_idx;
    CmTexture2D *texture[CM_TEXTURE_SLOTS];

    CmVbo vbo;
    CmVao vao;
    CmEbo ebo;

    size_t vertices_count;
    SpriteVertex data[CM_SPRITES_VERTICES_MAX];

    size_t indices_count;
    uint32_t indices[CM_SPRITES_INDICES_MAX];
  } sprite;

  struct LineRenderer {
    CmShader shader;

    CmVbo vbo;
    CmVao vao;
    CmEbo ebo;

    size_t vertices_count;
    LineVertex data[CM_LINES_VERTICES_MAX];
  } line;

  struct FontRenderer {
    CmVbo vbo;
    CmVao vao;

    CmShader shader;

    CmFont *font;

    FontVertex buffer[FONT_RENDERER_VERTECIES_MAX];
    size_t vertex_count;
  } font;
};

static CmRenderer2D *r;

/* ============= quad renderer ============= */

static void _cm_quad_internal_init(void) {
  r->quad.vbo = cm_gpu_vbo(&r->gpu, CM_DYNAMIC_DRAW, sizeof(QuadVertex),
                           CM_QUADS_VERTICES_MAX, (float *)&r->quad.data);

  r->quad.vao = cm_gpu_vao(&r->gpu);
  cm_gpu_vao_push(&r->quad.vao, 2, sizeof(QuadVertex),
                  offsetof(QuadVertex, pos));
  cm_gpu_vao_push(&r->quad.vao, 4, sizeof(QuadVertex),
                  offsetof(QuadVertex, color));

  u32 indices[CM_QUADS_INDICES] = {0, 1, 3, 1, 2, 3};
  for (size_t i = 0; i < CM_QUADS_INDICES_MAX; ++i) {
    r->quad.indices[i] = indices[i % CM_QUADS_INDICES];
    r->quad.indices[i] += (CM_QUADS_VERTICES * (i / CM_QUADS_INDICES));
  }
  r->quad.ebo = cm_gpu_ebo(&r->gpu, CM_STATIC_DRAW, CM_QUADS_INDICES_MAX,
                           r->quad.indices);

  r->quad.shader = cm_shader_from_memory(
      &r->gpu,
      STR("#version 330 core\n"
          "layout (location = 0) in vec2 a_pos;\n"
          "layout (location = 1) in vec4 a_color;\n"
          "uniform mat4 u_mvp;\n"
          "out vec4 v_color;\n"
          "void main() {\n"
          "  gl_Position = u_mvp * vec4(a_pos.xy, 0.0, 1.0);\n"
          "  v_color = a_color;\n"
          "}\n"),
      STR("#version 330 core\n"
          "in vec4 v_color;\n"
          "out vec4 f_color;\n"
          "void main() {\n"
          "  f_color = v_color;\n"
          "}\n"),
      ErrPanic);
}

static void _cm_quad_flush(void) {
  cm_shader_bind(&r->quad.shader);
  cm_shader_set_mat4(&r->quad.shader, STR("u_mvp"), r->camera->base.vp);

  cm_gpu_vbo_update(&r->quad.vbo, sizeof(QuadVertex), r->quad.vertices_count,
                    (float *)r->quad.data);

  cm_gpu_vao_bind(&r->quad.vao);

  cm_gpu_ebo_draw(&r->quad.ebo, r->quad.indices_count, CM_DRAW_TRIANGLES);

  r->quad.indices_count = 0;
  r->quad.vertices_count = 0;
}

void cm_quad(const vec2 pos, const vec2 s, float rotation, const vec4 color) {
  // flush if buffer full
  if (!(r->quad.vertices_count < CM_QUADS_VERTICES_MAX)) {
    _cm_quad_flush();
  }

  // For safety i still assert
  cebus_assert_debug(r->quad.vertices_count < CM_QUADS_VERTICES_MAX, "");
  cebus_assert_debug(r->quad.indices_count < CM_QUADS_INDICES_MAX, "");

  // cache cos_theta and sin_theta
  // its the same for every vertex
  float cos_theta = 1;
  float sin_theta = 0;
  if (rotation != 0) {
    cos_theta = cosf(rotation);
    sin_theta = sinf(rotation);
  }

  vec2 quad[CM_QUADS_VERTICES] = {
      {0, 0},
      {s[0], 0},
      {s[0], s[1]},
      {0, s[1]},
  };
  QuadVertex *vertices = &r->quad.data[r->quad.vertices_count];
  for (int i = 0; i < CM_QUADS_VERTICES; ++i) {
    if (rotation != 0) {
      const float x = quad[i][0];
      const float y = quad[i][1];
      quad[i][0] = x * cos_theta - y * sin_theta;
      quad[i][1] = x * sin_theta + y * cos_theta;
    }

    vertices[i].pos[0] = quad[i][0] + pos[0];
    vertices[i].pos[1] = quad[i][1] + pos[1];

    vertices[i].color[0] = color[0];
    vertices[i].color[1] = color[1];
    vertices[i].color[2] = color[2];
    vertices[i].color[3] = color[3];
  }

  r->quad.vertices_count += CM_QUADS_VERTICES;
  r->quad.indices_count += CM_QUADS_INDICES;
}

/* ============= circle renderer ============= */

static void _cm_circle_internal_init(void) {
  r->circle.vbo =
      cm_gpu_vbo(&r->gpu, CM_DYNAMIC_DRAW, sizeof(r->circle.vertices[0]),
                 CM_CIRCLES_MAX, NULL);

  r->circle.vao = cm_gpu_vao(&r->gpu);
  cm_gpu_vao_instanced(&r->circle.vao, 1, 2, sizeof(CircleVertex),
                       offsetof(CircleVertex, p));
  cm_gpu_vao_instanced(&r->circle.vao, 1, 1, sizeof(CircleVertex),
                       offsetof(CircleVertex, r));
  cm_gpu_vao_instanced(&r->circle.vao, 1, 4, sizeof(CircleVertex),
                       offsetof(CircleVertex, c));

  r->circle.shader = cm_shader_from_memory(
      &r->gpu,
      STR("#version 430 core\n"
          "layout (location = 0) in vec2 a_pos;\n"
          "layout (location = 1) in float a_radius;\n"
          "layout (location = 2) in vec4 a_color;\n"
          "uniform mat4 u_mvp;\n"
          "out vec2 v_uv;\n"
          "out vec4 v_color;\n"
          "void main() {\n"
          "  v_uv = vec2(gl_VertexID >> 0 & 1, gl_VertexID >> 1 & 1);\n"
          "  v_uv = 2 * v_uv - vec2(1.0, 1.0);\n"
          "  gl_Position = u_mvp * vec4(\n"
          "     a_pos + a_radius * v_uv,\n"
          "     0,\n"
          "     1);\n"
          "  v_color = a_color;"
          "}\n"),
      STR("#version 430 core\n"
          "in vec2 v_uv;\n"
          "in vec4 v_color;\n"
          "out vec4 f_color;\n"
          "void main() {\n"
          "  if (length(v_uv) < 1.0) {\n"
          "    f_color = v_color;\n"
          "  } else {\n"
          "    f_color = vec4(0, 0, 0, 0);\n"
          "  }\n"
          "}\n"),
      ErrPanic);
}

static void _cm_circle_flush(void) {
  cm_shader_bind(&r->circle.shader);
  cm_shader_set_mat4(&r->circle.shader, STR("u_mvp"), cm_camera_vp(r->camera));

  cm_gpu_vao_bind(&r->circle.vao);
  cm_gpu_vbo_update(&r->circle.vbo, sizeof(CircleVertex),
                    r->circle.vertex_count, (float *)r->circle.vertices);

  cm_gpu_vbo_draw_instanced(&r->circle.vbo, 4, CM_DRAW_TRIANGLE_STRIP);

  r->circle.vertex_count = 0;
}

void cm_circle(const vec2 pos, float radius, const vec4 color) {
  if (!(r->circle.vertex_count < CM_CIRCLES_MAX)) {
    _cm_circle_flush();
  }

  r->circle.vertices[r->circle.vertex_count].p[0] = pos[0];
  r->circle.vertices[r->circle.vertex_count].p[1] = pos[1];
  r->circle.vertices[r->circle.vertex_count].r = radius;
  r->circle.vertices[r->circle.vertex_count].c[0] = color[0];
  r->circle.vertices[r->circle.vertex_count].c[1] = color[1];
  r->circle.vertices[r->circle.vertex_count].c[2] = color[2];
  r->circle.vertices[r->circle.vertex_count].c[3] = color[3];

  r->circle.vertex_count++;
}

/* ============= sprite renderer ============= */

static void _cm_sprite_internal_init(void) {
  r->sprite.vbo = cm_gpu_vbo(&r->gpu, CM_DYNAMIC_DRAW, sizeof(SpriteVertex),
                             CM_SPRITES_VERTICES_MAX, (float *)&r->sprite.data);

  r->sprite.vao = cm_gpu_vao(&r->gpu);
  cm_gpu_vao_push(&r->sprite.vao, 2, sizeof(SpriteVertex),
                  offsetof(SpriteVertex, pos));
  cm_gpu_vao_push(&r->sprite.vao, 2, sizeof(SpriteVertex),
                  offsetof(SpriteVertex, uv));
  cm_gpu_vao_push(&r->sprite.vao, 1, sizeof(SpriteVertex),
                  offsetof(SpriteVertex, idx));

  u32 indices[CM_SPRITES_INDICES] = {0, 1, 3, 1, 2, 3};
  for (size_t i = 0; i < CM_SPRITES_INDICES_MAX; ++i) {
    r->sprite.indices[i] = indices[i % CM_SPRITES_INDICES];
    r->sprite.indices[i] += (CM_SPRITES_VERTICES * (i / CM_SPRITES_INDICES));
  }
  r->sprite.ebo = cm_gpu_ebo(&r->gpu, CM_STATIC_DRAW, CM_SPRITES_INDICES_MAX,
                             r->sprite.indices);

  r->sprite.shader = cm_shader_from_memory(
      &r->gpu,
      STR("#version 430 core\n"
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
      STR("#version 430 core\n"
          "in vec2 v_uv;\n"
          "out vec4 f_color;\n"
          "flat in int v_idx;\n"
          "uniform sampler2D u_sampler[8];\n"
          "void main() {\n"
          "  f_color = texture(u_sampler[v_idx], v_uv);\n"
          "}\n"),
      ErrPanic);

  cm_shader_bind(&r->sprite.shader);
  u32 loc = glGetUniformLocation(r->sprite.shader.id, "u_sampler");
  const int slots[8] = {0, 1, 2, 3, 4, 5, 6, 7};
  glUniform1iv(loc, ARRAY_LEN(slots), slots);
}

static void _cm_sprite_flush(void) {
  cm_shader_bind(&r->sprite.shader);
  cm_shader_set_mat4(&r->sprite.shader, STR("u_mvp"), r->camera->base.vp);

  for (usize i = 0; i < r->sprite.texture_idx; i++) {
    cm_texture_bind(r->sprite.texture[i], i);
  }

  cm_gpu_vbo_update(&r->sprite.vbo, sizeof(SpriteVertex),
                    r->sprite.vertices_count, (float *)r->sprite.data);

  cm_gpu_vao_bind(&r->sprite.vao);

  cm_gpu_ebo_draw(&r->sprite.ebo, r->sprite.indices_count, CM_DRAW_TRIANGLES);

  r->sprite.indices_count = 0;
  r->sprite.vertices_count = 0;

  r->sprite.texture_idx = 0;
}

static usize _cm_sprite_push_texture(CmTexture2D *texture) {
  if (CM_TEXTURE_SLOTS <= r->sprite.texture_idx) {
    _cm_sprite_flush();
  }

  for (usize i = 0; i < r->sprite.texture_idx; i++) {
    if (texture == r->sprite.texture[i]) {
      return i;
    }
  }
  r->sprite.texture[r->sprite.texture_idx++] = texture;
  return r->sprite.texture_idx - 1;
}

void cm_sprite(CmTexture2D *texture, const vec2 position, const vec2 size,
               float rotation, const vec2 uv, const vec2 uv_size) {
  usize idx = _cm_sprite_push_texture(texture);

  if (!(r->sprite.vertices_count < CM_SPRITES_VERTICES_MAX)) {
    _cm_sprite_flush();
  }

  // For safety i still assert
  cebus_assert_debug(r->sprite.vertices_count < CM_SPRITES_VERTICES_MAX, "");
  cebus_assert_debug(r->sprite.indices_count < CM_SPRITES_INDICES_MAX, "");

  // cache the cos_theta and sin_theta
  // its the same for every vertex
  float cos_theta = 1;
  float sin_theta = 0;
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
  SpriteVertex *vertices = &r->sprite.data[r->sprite.vertices_count];
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

  r->sprite.vertices_count += CM_SPRITES_VERTICES;
  r->sprite.indices_count += CM_SPRITES_INDICES;
}

/* ============= line renderer ============= */

static void _cm_line_internal_init(void) {
  r->line.vbo = cm_gpu_vbo(&r->gpu, CM_DYNAMIC_DRAW, sizeof(LineVertex),
                           CM_LINES_VERTICES_MAX, (float *)&r->line.data);

  r->line.vao = cm_gpu_vao(&r->gpu);
  cm_gpu_vao_push(&r->line.vao, 2, sizeof(LineVertex),
                  offsetof(LineVertex, pos));

  r->line.shader = cm_shader_from_memory(
      &r->gpu,
      STR("#version 330 core\n"
          "layout (location = 0) in vec2 a_pos;\n"
          "uniform mat4 u_mvp;\n"
          "void main() {\n"
          "  gl_Position = u_mvp * vec4(a_pos.xy, 0.0, 1.0);\n"
          "}\n"),
      STR("#version 330 core\n"
          "uniform vec4 u_color = vec4(1, 1, 1, 1);\n"
          "out vec4 f_color;\n"
          "void main() {\n"
          "  f_color = u_color;\n"
          "}\n"),
      ErrPanic);
}

static void _cm_line_flush(void) {
  cm_shader_bind(&r->line.shader);
  cm_shader_set_mat4(&r->line.shader, STR("u_mvp"), r->camera->base.vp);

  cm_gpu_vbo_update(&r->line.vbo, sizeof(LineVertex), r->line.vertices_count,
                    (float *)r->line.data);

  cm_gpu_vao_bind(&r->line.vao);
  cm_gpu_vbo_draw(&r->line.vbo, CM_DRAW_LINES);

  r->line.vertices_count = 0;
}

void cm_line(const vec2 from, const vec2 to) {
  if (!(r->line.vertices_count < CM_LINES_VERTICES_MAX)) {
    _cm_line_flush();
  }

  // For safety i still assert
  cebus_assert_debug(r->line.vertices_count < CM_LINES_VERTICES_MAX, "");

  LineVertex *vertices = &r->line.data[r->line.vertices_count];

  vertices[0].pos[0] = from[0];
  vertices[0].pos[1] = from[1];

  vertices[1].pos[0] = to[0];
  vertices[1].pos[1] = to[1];

  r->line.vertices_count += CM_LINES_VERTICES;
}

/* ============= font renderer ============= */

static void _cm_font_internal_init(void) {
  r->font.shader = cm_shader_from_memory(
      &r->gpu,
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
          "uniform vec4 u_color = vec4(1, 1, 1, 1);"
          "void main() {\n"
          " gl_FragColor = vec4(texture2D(u_texture, v_uv).r) * u_color;\n"
          "}\n"),
      ErrDefault);

  r->font.vbo = cm_gpu_vbo(
      &r->gpu, CM_DYNAMIC_DRAW, sizeof(FontVertex),
      FONT_RENDERER_VERTECIES_PER_CHAR * FONT_RENDERER_CHAR_MAX, NULL);

  r->font.vao = cm_gpu_vao(&r->gpu);
  cm_gpu_vao_push(&r->font.vao, 2, sizeof(FontVertex),
                  offsetof(FontVertex, pos));
  cm_gpu_vao_push(&r->font.vao, 2, sizeof(FontVertex),
                  offsetof(FontVertex, uv));
}

static void _cm_font_renderer_flush(void) {
  cm_shader_bind(&r->font.shader);
  cm_shader_set_mat4(&r->font.shader, STR("u_mvp"), r->camera->base.vp);
  cm_shader_set_i32(&r->font.shader, STR("u_texture"), 0);
  cm_shader_set_vec4(&r->font.shader, STR("u_color"), r->font.font->color);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, r->font.font->texture_id);

  glBindBuffer(GL_ARRAY_BUFFER, r->font.vbo.id);
  glBindVertexArray(r->font.vao.id);

  cm_gpu_vbo_update(&r->font.vbo, sizeof(FontVertex), r->font.vertex_count,
                    (float *)r->font.buffer);

  glDrawArrays(GL_TRIANGLES, 0, r->font.vertex_count);

  r->font.vertex_count = 0;
  r->font.font = NULL;
}

static void _cm_push_char(CmFont *font, char c, FontVertex *v, float *x,
                          float *y) {
  stbtt_aligned_quad q;
  stbtt_GetBakedQuad(font->cdata, font->ttf_resoulution, font->ttf_resoulution,
                     c - FONT_CHAR_MIN, x, y, &q, 1);

  usize idx = 0;
  glm_vec2_copy((vec2){q.x0, q.y0}, v[idx].pos);
  glm_vec2_copy((vec2){q.s0, q.t0}, v[idx].uv);
  idx++;

  glm_vec2_copy((vec2){q.x1, q.y0}, v[idx].pos);
  glm_vec2_copy((vec2){q.s1, q.t0}, v[idx].uv);
  idx++;

  glm_vec2_copy((vec2){q.x1, q.y1}, v[idx].pos);
  glm_vec2_copy((vec2){q.s1, q.t1}, v[idx].uv);
  idx++;

  glm_vec2_copy((vec2){q.x0, q.y1}, v[idx].pos);
  glm_vec2_copy((vec2){q.s0, q.t1}, v[idx].uv);
  idx++;

  glm_vec2_copy((vec2){q.x0, q.y0}, v[idx].pos);
  glm_vec2_copy((vec2){q.s0, q.t0}, v[idx].uv);
  idx++;

  glm_vec2_copy((vec2){q.x1, q.y1}, v[idx].pos);
  glm_vec2_copy((vec2){q.s1, q.t1}, v[idx].uv);
  idx++;

  r->font.vertex_count += FONT_RENDERER_VERTECIES_PER_CHAR;
}

void cm_font(CmFont *font, const vec2 pos, Str text) {
  // if font passed not the same as last font.
  if (r->font.font && r->font.font != font) {
    _cm_font_renderer_flush();
  }
  r->font.font = font;

  float text_y = pos[1] + font->height;
  float text_x = pos[0];
  FontVertex *vertex = &r->font.buffer[r->font.vertex_count];
  for (size_t i = 0; i < text.len; i++) {
    if (FONT_CHAR_MIN <= text.data[i] &&
        text.data[i] < FONT_CHAR_MIN + FONT_CHAR_MAX - 1) {

      if (!(r->font.vertex_count < FONT_RENDERER_VERTECIES_MAX)) {
        _cm_font_renderer_flush();
        vertex = r->font.buffer;
      }

      _cm_push_char(font, text.data[i], vertex, &text_x, &text_y);
      vertex += FONT_RENDERER_VERTECIES_PER_CHAR;
    } else if (text.data[i] == '\n') {
      text_y += font->height;
      text_x = pos[0];
    } else if (text.data[i] == '\t') {
      for (usize i = 0; i < 4; ++i) {
        _cm_push_char(font, ' ', vertex, &text_x, &text_y);
        vertex += FONT_RENDERER_VERTECIES_PER_CHAR;
      }
    }
  }
}

void cm_font_color(CmFont *font, vec4 color) {
  glm_vec4_copy(color, font->color);
}

/* ============= renderer 2D ============= */

void cm_2D_begin(CmCamera2D *camera) {
  if (r->camera != NULL && r->camera != camera) {
    cebus_log_error(
        FILE_LOCATION_FMT
        ": 'cm_2D_begin()' was called twice before 'cm_2D_end()' was called",
        FILE_LOCATION_ARG_CURRENT);
    DEBUGBREAK();
  }
  r->camera = camera;
  cm_camera_update(camera);

  glDisable(GL_DEPTH_TEST);
}

void cm_2D_end(void) {
  if (r->camera == NULL) {
    cebus_log_error(FILE_LOCATION_FMT ": 'cm_2D_begin()' was never called",
                    FILE_LOCATION_ARG_CURRENT);
    DEBUGBREAK();
  }

  if (r->quad.vertices_count) {
    _cm_quad_flush();
  }

  if (r->circle.vertex_count) {
    _cm_circle_flush();
  }

  if (r->sprite.vertices_count) {
    _cm_sprite_flush();
  }

  if (r->line.vertices_count) {
    _cm_line_flush();
  }

  if (r->font.vertex_count) {
    _cm_font_renderer_flush();
  }

  r->camera = NULL;
  glEnable(GL_DEPTH_TEST);
}

void cm_2D_internal_set_context(CmRenderer2D *renderer) {
  cebus_assert(renderer, "renderer was not initialized");
  cebus_assert(r == NULL, "can not have two initialized renderers");
  r = renderer;
}

CmRenderer2D *cm_2D_internal_init(void) {
  r = calloc(1, sizeof(CmRenderer2D));
  r->gpu = cm_gpu_internal_init(&r->arena);

  _cm_quad_internal_init();
  _cm_circle_internal_init();
  _cm_sprite_internal_init();
  _cm_line_internal_init();
  _cm_font_internal_init();

  return r;
}

void cm_2D_internal_free(void) {
  cm_gpu_internal_free(&r->gpu);
  arena_free(&r->arena);
  free(r);
}
