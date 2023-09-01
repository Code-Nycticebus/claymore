#include "claymore.h"
#include "claymore/renderer/render_command.h"

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

static struct ShaderData grid_shader;
static struct ShaderData framebuffer_shader;

static CmFont *font;

static mat4 model = GLM_MAT4_IDENTITY_INIT;

static float zoom = 100.F;
static float aspect;

uint32_t fbo;
uint32_t texture;
uint32_t rbo;
CmVertexAttribute attributes;

static void ortho_scroll_callback(CmScrollEvent *event, CmLayer *layer) {
  const float min_zoom = 1.F;
  const float scroll_speed = 100.F;
  zoom = glm_max(zoom - event->yoffset * scroll_speed, min_zoom);
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
  grid_shader.id = cm_load_shader_from_file("res/shader/basic.vs.glsl",
                                            "res/shader/basic.fs.glsl");
  grid_shader.uniform_loc.mvp =
      cm_shader_get_uniform_location(grid_shader.id, "u_mvp");

  aspect = (float)layer->app->window->width / (float)layer->app->window->height;
  glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 100.F,
            layer->camera.projection);
  glm_mat4_identity(layer->camera.view);
  glm_vec3_copy((vec3){0}, layer->camera.position);
  glm_translate(layer->camera.view, layer->camera.position);
  layer->camera.update = true;

  const float font_size = 101.F;
  font = cm_font_init("res/fonts/Ubuntu.ttf", font_size);

  cm_event_set_callback(CM_EVENT_WINDOW_RESIZE,
                        (cm_event_callback)ortho_window_resize_callback,
                        &layer->camera);
  cm_event_set_callback(CM_EVENT_MOUSE, (cm_event_callback)ortho_mouse_callback,
                        &layer->camera);
  cm_event_set_callback(CM_EVENT_SCROLL,
                        (cm_event_callback)ortho_scroll_callback, layer);

  cm_event_set_callback(CM_EVENT_KEYBOARD,
                        (cm_event_callback)ortho_key_callback, layer);

  /* Frame buffer */
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  // Texture to render the framebuffer to
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, layer->app->window->width,
               layer->app->window->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);

  // Specify the attachment if not color attachment.
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture, 0);

  // Render buffer object
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);

  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                        layer->app->window->width, layer->app->window->height);

  // Attach the renderbuffer to framebuffer
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    cm_log_error("Frame buffer was not initialized correctly!\n");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  const float vertecies[] = {
      // positions            // texCoords
      -1.0f, 1.0f,  0.0f, 1.0f, //
      -1.0f, -1.0f, 0.0f, 0.0f, //
      1.0f,  -1.0f, 1.0f, 0.0f, //

      -1.0f, 1.0f,  0.0f, 1.0f, //
      1.0f,  -1.0f, 1.0f, 0.0f, //
      1.0f,  1.0f,  1.0f, 1.0f, //
  };

  CmVertexBuffer buffer =
      cm_vertex_buffer_create(6, 4 * sizeof(float), vertecies, GL_STATIC_DRAW);
  attributes = cm_vertex_attribute_create(&buffer);
  cm_vertex_attribute_push(&attributes, 2, GL_FLOAT, 0);
  cm_vertex_attribute_push(&attributes, 2, GL_FLOAT, sizeof(float) * 2);

  framebuffer_shader.id = cm_load_shader_from_file(
      "res/shader/framebuffer.vs.glsl", "res/shader/framebuffer.fs.glsl");
  framebuffer_shader.uniform_loc.texture =
      glGetUniformLocation(framebuffer_shader.id, "screen_texture");

  cm_renderer_set_clear_color((vec4){0});
}

static void ortho_update(CmLayer *layer, float dt) {
  (void)dt, (void)layer, (void)model;

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glClear(GL_COLOR_BUFFER_BIT |
          GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now

  static mat4 mvp;
  glm_mat4_mul(layer->camera.vp, model, mvp);

  glUseProgram(grid_shader.id);
  glUniformMatrix4fv(grid_shader.uniform_loc.mvp, 1, GL_FALSE, (float *)mvp);

  cm_renderer2d_begin();
  static uint32_t grid_size = 100; // 317^2 == 100'000 quads
  const float quad_size = 5.F;
  static float rotation = 0.F;
  const float rotation_speed = 45.F;
  rotation += rotation_speed * dt;
  for (size_t i = 0; i < grid_size; i++) {
    for (size_t j = 0; j < grid_size; j++) {
      cm_renderer2d_push_quad_color_rotated(
          (vec2){i * (quad_size + quad_size / 2),
                 j * (quad_size + quad_size / 2)},
          0.F, (vec2){quad_size, quad_size}, (vec4){0.F, 0.F, 1.F, 1.F},
          glm_rad(rotation + i + j));
    }
  }
  cm_renderer2d_end();

  glUseProgram(0);

#define LABEL_SIZE 128
  char label_buffer[LABEL_SIZE];
  const size_t len =
      snprintf(label_buffer, LABEL_SIZE - 1, "Batch renderer: %u quads",
               grid_size * grid_size);
  cm_font_draw(font, mvp, 0.F, -100.F, 1.F, len, label_buffer);

  // Set the framebuffer to the screens frame buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glUseProgram(framebuffer_shader.id);
  {
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(attributes.id);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(framebuffer_shader.uniform_loc.texture, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
  }
  glUseProgram(0);
}

static void ortho_free(CmLayer *layer) {
  (void)layer;
  glDeleteFramebuffers(1, &fbo);
}

CmLayerInterface sandbox_ortho(void) {
  return (CmLayerInterface){
      .init = ortho_init,
      .free = ortho_free,
      .update = ortho_update,
  };
}
