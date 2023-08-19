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

static struct ShaderData background_shader;
static Texture background_texture;

static mat4 model = GLM_MAT4_IDENTITY_INIT;

static float zoom = 1.F;
static float aspect;

static void ortho_scroll_callback(CmScrollEvent *event, CmLayer *layer) {
  const float min_zoom = 0.1F;
  zoom = glm_max(zoom - event->yoffset, min_zoom);
  glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 1.F,
            layer->camera.projection);
  layer->camera.update = true;
}

static void ortho_window_resize_callback(CmWindowEvent *event,
                                         CmCamera *camera) {
  aspect = (float)event->window->width / (float)event->window->height;
  glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 1.F,
            camera->projection);
  camera->update = true;
}

static void ortho_mouse_callback(CmMouseEvent *event, CmLayer *layer) {
  if (event->action == CM_MOUSE_MOVE) {
    static vec3 last_position = {0};
    vec2 pos;
    vec2 direction;
    cm_mouseinfo_pos(pos);
    glm_vec2_sub(pos, last_position, direction);
    glm_vec2_copy(pos, last_position);

    if (cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
      glm_vec2_scale(direction, zoom / 100.F, direction);
      glm_vec2_add(layer->camera.position, direction, layer->camera.position);

      glm_mat4_identity(layer->camera.view);
      glm_translate(layer->camera.view, layer->camera.position);
      layer->camera.update = true;
    }
  }
}

static void ortho_key_callback(CmKeyEvent *event, CmLayer *layer) {
  if (event->action == CM_KEY_PRESS) {
    event->base.handled = true;
    switch (event->code) {
    case CM_KEY_F5: {
      cm_camera_position(&layer->camera, (vec3){0, 0, 4});
      break;
    }
    case CM_KEY_ESCAPE: {
      // TODO cm_app_close()
      cm_event_dispatch((CmEvent){
          .type = CM_EVENT_WINDOW_CLOSE,
          .event.window.window = layer->app->window,
      });
      break;
    }

    case CM_KEY_F2: {
      static bool vsync = true;
      vsync = !vsync;
      glfwSwapInterval(vsync);
      cm_log_info("vsync turned %s\n", vsync ? "on" : "off");
      break;
    }
    default:
      event->base.handled = false;
      break;
    }
  }
}

static void background_init(CmLayer *layer) {
  background_shader.id = cm_load_shader_from_file("res/shader/texture.vs.glsl",
                                                  "res/shader/texture.fs.glsl");
  background_shader.uniform_loc.mvp =
      cm_shader_get_uniform_location(background_shader.id, "u_mvp");
  background_shader.uniform_loc.texture =
      cm_shader_get_uniform_location(background_shader.id, "u_texture");

  background_texture = cm_texture2d_create("res/textures/claymore-sword.png");

  aspect = (float)layer->app->window->width / (float)layer->app->window->height;
  glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 100.F,
            layer->camera.projection);
  glm_mat4_identity(layer->camera.view);
  glm_vec3_copy((vec3){0}, layer->camera.position);
  glm_translate(layer->camera.view, layer->camera.position);
  layer->camera.update = true;

  cm_event_set_callback(CM_EVENT_WINDOW_RESIZE,
                        (cm_event_callback)ortho_window_resize_callback,
                        &layer->camera);
  cm_event_set_callback(CM_EVENT_MOUSE, (cm_event_callback)ortho_mouse_callback,
                        &layer->camera);
  cm_event_set_callback(CM_EVENT_SCROLL,
                        (cm_event_callback)ortho_scroll_callback, layer);

  cm_event_set_callback(CM_EVENT_KEYBOARD,
                        (cm_event_callback)ortho_key_callback, layer);
}

static void background_update(CmLayer *layer, float dt) {
  (void)dt;

  static mat4 mvp;
  glm_mat4_mul(layer->camera.vp, model, mvp);

  cm_texture_bind(&background_texture, 0);

  glUseProgram(background_shader.id);
  glUniformMatrix4fv(background_shader.uniform_loc.mvp, 1, GL_FALSE,
                     (float *)mvp);
  glUniform1i(background_shader.uniform_loc.texture, 0);

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

static void background_free(CmLayer *layer) {
  (void)layer;
  cm_texture_delete(&background_texture);
}

CmLayerInterface sandbox_background(void) {
  return (CmLayerInterface){
      .init = background_init,
      .free = background_free,
      .update = background_update,
  };
}
