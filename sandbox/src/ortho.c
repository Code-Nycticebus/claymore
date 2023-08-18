#include "cglm/vec3.h"
#include "claymore.h"

#include "claymore/renderer/renderer2D.h"
#include "stb_image.h"

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

static struct ShaderData ortho_shader;
static struct ShaderData grid_shader;

static mat4 model = GLM_MAT4_IDENTITY_INIT;

static uint32_t texture_id;

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

static void ortho_init(CmLayer *layer) {
  ortho_shader.id = cm_load_shader_from_file("res/shader/texture.vs.glsl",
                                             "res/shader/texture.fs.glsl");
  ortho_shader.uniform_loc.mvp =
      cm_shader_get_uniform_location(ortho_shader.id, "u_mvp");
  ortho_shader.uniform_loc.texture =
      cm_shader_get_uniform_location(ortho_shader.id, "u_texture");

  grid_shader.id = cm_load_shader_from_file("res/shader/basic.vs.glsl",
                                            "res/shader/basic.fs.glsl");
  grid_shader.uniform_loc.mvp =
      cm_shader_get_uniform_location(grid_shader.id, "u_mvp");

  int32_t texture_width = 0;
  int32_t texture_height = 0;
  int32_t bits_per_pixel;

  stbi_set_flip_vertically_on_load(true);
  unsigned char *texture_buffer =
      stbi_load("res/textures/claymore-sword.png", &texture_width,
                &texture_height, &bits_per_pixel, 4);
  const char *fail = stbi_failure_reason();
  if (fail) {
    cm_log_error("%s\n", fail);
  }

  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture_width, texture_height, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, texture_buffer);

  glBindTexture(GL_TEXTURE_2D, 0);

  if (texture_buffer) {
    stbi_image_free(texture_buffer);
  }

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

  cm_renderer_set_clear_color((vec4){1.F, 1.F, 0.F, 1.F});
}

static void ortho_update(CmLayer *layer, float dt) {
  (void)dt;

  static mat4 mvp;
  glm_mat4_mul(layer->camera.vp, model, mvp);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glUseProgram(ortho_shader.id);
  glUniformMatrix4fv(ortho_shader.uniform_loc.mvp, 1, GL_FALSE, (float *)mvp);
  glUniform1i(ortho_shader.uniform_loc.texture, 0);

  const float background_size = 100000.F;
  cm_renderer2d_begin();
  cm_renderer2d_push_quad((vec2){-background_size/2, -background_size/2}, -0.1F,
                          (vec2){background_size, background_size}, (vec2){0.F, 0.F},
                          (vec2){background_size, background_size});
  cm_renderer2d_end();

  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, 0);

  glUseProgram(grid_shader.id);
  glUniformMatrix4fv(grid_shader.uniform_loc.mvp, 1, GL_FALSE, (float *)mvp);
  cm_renderer2d_begin();
  const size_t grid_size = 100;
  const float quad_size = 0.05F;
  for (size_t i = 0; i < grid_size; i++) {
      for (size_t j = 0; j < grid_size; j++) {
      cm_renderer2d_push_quad_color((vec2){i * (quad_size + quad_size / 4),
                                           j * (quad_size + quad_size / 4)},
                                    0.F, (vec2){quad_size, quad_size},
                                    (vec4){0.F, 0.F, 1.F, 1.F});
    }
  }

  cm_renderer2d_end();
  glUseProgram(0);
}

static void ortho_free(CmLayer *layer) {
  (void)layer;
  glDeleteTextures(1, &texture_id);
}

CmLayerInterface sandbox_ortho(void) {
  return (CmLayerInterface){
      .init = ortho_init,
      .free = ortho_free,
      .update = ortho_update,
  };
}
