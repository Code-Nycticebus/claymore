#include "claymore.h"

struct Vertex {
  vec3 pos;
  vec2 uv;
};

struct ShaderData {
  uint32_t id;

  struct {
    GLint mvp;
    GLint texture;
  } uniform_loc;
};

static CmShader background_shader;
static Texture background_texture;

static mat4 model = GLM_MAT4_IDENTITY_INIT;

static float zoom = 1.F;
static float aspect;

static void background_scroll_callback(CmScrollEvent *event, CmLayer *layer) {
  const float min_zoom = 0.1F;
  zoom = glm_max(zoom - event->yoffset, min_zoom);
  glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 1.F,
            layer->camera.projection);
  layer->camera.update = true;
}

static void background_window_resize_callback(CmWindowEvent *event,
                                              CmCamera *camera) {
  aspect = (float)event->window->width / (float)event->window->height;
  glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 1.F,
            camera->projection);
  camera->update = true;
}

static void background_mouse_callback(CmMouseEvent *event, CmLayer *layer) {
  if (event->action == CM_MOUSE_MOVE) {
    static vec3 last_position = {0};
    vec2 pos;
    vec2 direction;
    cm_mouseinfo_pos(pos);
    glm_vec2_sub(pos, last_position, direction);
    glm_vec2_copy(pos, last_position);

    if (cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
      const float zoom_scale = 100.F;
      glm_vec2_scale(direction, zoom / zoom_scale, direction);
      glm_vec2_add(layer->camera.position, direction, layer->camera.position);

      glm_mat4_identity(layer->camera.view);
      glm_translate(layer->camera.view, layer->camera.position);
      layer->camera.update = true;
    }
  }
}

static bool background_init(CmScene *scene, CmLayer *layer) {
  background_shader = cm_load_shader_from_file("res/shader/texture.vs.glsl",
                                               "res/shader/texture.fs.glsl");

  background_texture = cm_texture2d_create("res/textures/claymore-sword.png");

  aspect = (float)scene->app->window->width / (float)scene->app->window->height;
  glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 100.F,
            layer->camera.projection);
  glm_mat4_identity(layer->camera.view);
  glm_vec3_copy((vec3){0}, layer->camera.position);
  glm_translate(layer->camera.view, layer->camera.position);
  layer->camera.update = true;

  cm_event_set_callback(CM_EVENT_WINDOW_RESIZE,
                        (cm_event_callback)background_window_resize_callback,
                        &layer->camera);
  cm_event_set_callback(CM_EVENT_MOUSE,
                        (cm_event_callback)background_mouse_callback,
                        &layer->camera);
  cm_event_set_callback(CM_EVENT_SCROLL,
                        (cm_event_callback)background_scroll_callback, layer);

  const float scale = 10.F;
  glm_scale(model, (vec3){scale, scale, 1.F});
  return true;
}

static void background_update(CmScene *scene, CmLayer *layer, float dt) {
  (void)dt, (void)scene;

  static mat4 mvp;
  glm_mat4_mul(layer->camera.vp, model, mvp);

  cm_texture_bind(&background_texture, 0);

  cm_shader_bind(&background_shader);
  cm_shader_set_mat4(&background_shader, "u_mvp", mvp);
  cm_shader_set_i32(&background_shader, "u_texture", 0);

  const float background_size = 100000.F;
  const float background_layer = -0.99F;
  cm_renderer2d_begin();
  cm_renderer2d_push_quad(
      (vec2){-background_size / 2, -background_size / 2}, background_layer,
      (vec2){background_size, background_size}, (vec2){0.F, 0.F},
      (vec2){background_size, background_size});
  cm_renderer2d_end();
  glUseProgram(0);
  cm_texture_unbind(0);
}

static void background_free(CmScene *scene, CmLayer *layer) {
  (void)layer, (void)scene;
  cm_texture_delete(&background_texture);
  cm_shader_delete(&background_shader);
}

CmLayerInterface sandbox_background(void) {
  return (CmLayerInterface){
      .init = background_init,
      .free = background_free,
      .update = background_update,
  };
}
