#include "claymore.h"

struct Vertex {
  vec3 pos;
  vec2 uv;
};

struct ShaderData {
  uint32_t id;

  struct {
    GLint mvp;
  } uniform_loc;
};

static struct ShaderData ortho_shader;

static mat4 model = GLM_MAT4_IDENTITY_INIT;

static CmRenderBuffer render_data;

#define INITIAL_ZOOM 10.F
static float zoom = INITIAL_ZOOM;

static float gcd(uint32_t a, uint32_t b) { return b == 0 ? a : gcd(b, a % b); }

typedef struct {
  uint32_t w;
  uint32_t h;
} Aspect;

static Aspect get_aspect(uint32_t width, uint32_t height) {
  float divisor = gcd(width, height);
  return (Aspect){.w = width / divisor, .h = height / divisor};
}

static void ortho_scroll_callback(CmScrollEvent *event, CmLayer *layer) {
  zoom = glm_max(zoom + event->yoffset * 2, 1.F);
  Aspect aspect = get_aspect(event->window->width, event->window->height);
  glm_ortho(-zoom * aspect.w, zoom * aspect.w, -zoom * aspect.h,
            zoom * aspect.h, -100.F, 100.F, layer->camera.projection);
  layer->camera.update = true;
}

static void ortho_window_resize_callback(CmWindowEvent *event,
                                         CmCamera *camera) {
  Aspect aspect = get_aspect(event->window->width, event->window->height);
  glm_ortho(-zoom * aspect.w, zoom * aspect.w, -zoom * aspect.h,
            zoom * aspect.h, -100.F, 100.F, camera->projection);
  camera->update = true;
}

static void ortho_init(CmLayer *layer) {
  ortho_shader.id = cm_load_shader_from_file("res/shader/texture.vs.glsl",
                                             "res/shader/texture.fs.glsl");
  ortho_shader.uniform_loc.mvp =
      cm_shader_get_uniform_location(ortho_shader.id, "u_mvp");
  ortho_shader.uniform_loc.texture =
      cm_shader_get_uniform_location(ortho_shader.id, "u_texture");

  int32_t texture_width = 0;
  int32_t texture_height = 0;
  int32_t bits_per_pixel;

  stbi_set_flip_vertically_on_load(true);
  unsigned char *texture_buffer =
      stbi_load("res/textures/apple.png", &texture_width, &texture_height,
                &bits_per_pixel, 4);
  const char *fail = stbi_failure_reason();
  if (fail) {
    cm_log_error("%s\n", fail);
  }

  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture_width, texture_height, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, texture_buffer);

  glBindTexture(GL_TEXTURE_2D, 0);

  if (texture_buffer) {
    stbi_image_free(texture_buffer);
  }

  Aspect aspect =
      get_aspect(layer->app->window->width, layer->app->window->height);
  glm_ortho(-zoom * aspect.w, zoom * aspect.w, -zoom * aspect.h,
            zoom * aspect.h, -100.F, 100.F, layer->camera.projection);
  glm_mat4_identity(layer->camera.view);
  layer->camera.update = true;

  cm_event_set_callback(CM_EVENT_WINDOW_RESIZE,
                        (cm_event_callback)ortho_window_resize_callback,
                        &layer->camera);
  cm_event_set_callback(CM_EVENT_SCROLL,
                        (cm_event_callback)ortho_scroll_callback, layer);

  struct Vertex vertecies[] = {
      {{-100.F, -100.F, 0.F}, {0.F, 0.F}}, //
      {{100.F, -100.F, 0.F}, {1.F, 0.F}},  //
      {{100.F, 100.F, 0.F}, {1.F, 1.F}},   //
      {{-100.F, 100.F, 0.F}, {0.F, 1.F}},
  };
  const size_t vertecies_count = 4;

  render_data.vertex_buffer = cm_vertex_buffer_create(
      vertecies_count, sizeof(struct Vertex), vertecies, GL_STATIC_DRAW);

  render_data.vertex_attribute =
      cm_vertex_attribute_create(&render_data.vertex_buffer);
  cm_vertex_attribute_push(&render_data.vertex_attribute, 3, GL_FLOAT,
                           offsetof(struct Vertex, pos));
  cm_vertex_attribute_push(&render_data.vertex_attribute, 2, GL_FLOAT,
                           offsetof(struct Vertex, uv));

  uint32_t indices[] = {0, 1, 2, 0, 2, 3};
  const uint32_t indices_count = 6;
  render_data.index_buffer = cm_index_buffer_create(
      &render_data.vertex_attribute, indices_count, indices, GL_STATIC_DRAW);
}

static void ortho_update(CmLayer *layer, float dt) {
  (void)dt;

  static mat4 mvp;
  glm_mat4_mul(layer->camera.vp, model, mvp);

  glUseProgram(ortho_shader.id);
  glUniformMatrix4fv(ortho_shader.uniform_loc.mvp, 1, GL_FALSE, (float *)mvp);

  cm_renderer_draw_indexed(&render_data, render_data.index_buffer.count);

  glUseProgram(0);
}

static void ortho_free(CmLayer *layer) { (void)layer; }

CmLayerInterface sandbox_ortho(void) {
  return (CmLayerInterface){
      .init = ortho_init,
      .free = ortho_free,
      .update = ortho_update,
  };
}
