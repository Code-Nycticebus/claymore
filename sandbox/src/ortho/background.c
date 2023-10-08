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
static CmTexture2D background_texture;

static mat4s model = GLMS_MAT4_IDENTITY_INIT;

static float aspect;

static void background_scroll_callback(CmScrollEvent *event, CmLayer *layer) {
  const float min_zoom = 0.1F;
  const float scroll_speed = 10.F;
  float zoom = layer->camera.zoom;
  zoom = glm_max(zoom - event->yoffset * (zoom / scroll_speed), min_zoom);
  cm_camera_zoom(&layer->camera, zoom);
}

static void background_window_resize_callback(CmWindowEvent *event,
                                              CmCamera *camera) {
  aspect = (float)event->window->width / (float)event->window->height;
  cm_camera_aspect(camera, aspect);
}

static void background_mouse_callback(CmMouseEvent *event, CmLayer *layer) {
  if (event->action == CM_MOUSE_MOVE) {
    static vec2s last_position = {0};
    vec2s pos = cm_mouseinfo_pos();
    vec2s direction = glms_vec2_sub(pos, last_position);
    last_position = pos;

    if (cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
      const float zoom_scale = 150.F;
      direction = glms_vec2_scale(direction, layer->camera.zoom / zoom_scale);
      layer->camera.position = glms_vec3_add(
          layer->camera.position, (vec3s){{direction.x, direction.y, 0}});

      layer->camera.view = glms_mat4_identity();
      layer->camera.view =
          glms_translate(layer->camera.view, layer->camera.position);
      layer->camera.update = true;
    }
  }
}

static bool background_init(CmScene *scene, CmLayer *layer) {
  background_shader = cm_shader_load_from_file("res/shader/texture.vs.glsl",
                                               "res/shader/texture.fs.glsl");

  background_texture = cm_texture2d_create("res/textures/claymore-sword.png");

  aspect = (float)scene->app->window->width / (float)scene->app->window->height;
  layer->camera = cm_camera_init_ortho((vec3s){0}, aspect, 100.F);

  cm_event_subscribe(CM_EVENT_WINDOW_RESIZE,
                     (cm_event_callback)background_window_resize_callback,
                     &layer->camera);
  cm_event_subscribe(CM_EVENT_MOUSE,
                     (cm_event_callback)background_mouse_callback,
                     &layer->camera);
  cm_event_subscribe(CM_EVENT_SCROLL,
                     (cm_event_callback)background_scroll_callback, layer);

  const float scale = 10.F;
  model = glms_scale(model, (vec3s){{scale, scale, 1.F}});
  return true;
}

static void background_update(CmScene *scene, CmLayer *layer, float dt) {
  (void)dt, (void)scene;

  static mat4s mvp;
  mvp = glms_mat4_mul(layer->camera.vp, model);

  cm_texture_bind(&background_texture, 0);

  cm_shader_bind(&background_shader);
  cm_shader_set_mat4(&background_shader, "u_mvp", mvp);
  cm_shader_set_i32(&background_shader, "u_texture", 0);

  const float background_size = 100000.F;
  const float background_layer = -0.99F;
  cm_renderer2d_begin();
  cm_renderer2d_push_quad_textured(
      (vec2s){{-background_size / 2, -background_size / 2}}, background_layer,
      (vec2s){{background_size, background_size}}, (vec2s){{0.F, 0.F}},
      (vec2s){{background_size, background_size}});
  cm_renderer2d_end();
  cm_shader_unbind();
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
