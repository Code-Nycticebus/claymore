#include "claymore.h"

struct Vertex {
  vec3 pos;
  vec2 uv;
};

static CmShader grid_shader;

static CmFont *font;
static const float font_size = 64.F;
static mat4 model = GLM_MAT4_IDENTITY_INIT;

#define ORTHO_INITIAL_ZOOM 1400.F
static float zoom = ORTHO_INITIAL_ZOOM;
static vec3 camera_initial_position = {0, 0, 0};
static float aspect;
static vec3 mouse_last_position = {0};

static void ortho_scroll_callback(CmScrollEvent *event, CmCamera *camera) {
  const float min_zoom = 1.F;
  const float scroll_speed = 100.F;
  zoom = glm_max(zoom - event->yoffset * scroll_speed, min_zoom);
  glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 1.F,
            camera->projection);
  camera->update = true;
}

static void ortho_window_resize_callback(CmWindowEvent *event,
                                         CmCamera *camera) {
  aspect = (float)event->window->width / (float)event->window->height;
  glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 1.F,
            camera->projection);
  camera->update = true;
}

static void ortho_mouse_callback(CmMouseEvent *event, CmCamera *camera) {
  if (event->action == CM_MOUSE_MOVE) {

    vec2 pos;
    vec2 direction;
    cm_mouseinfo_pos(pos);
    glm_vec2_sub(pos, mouse_last_position, direction);
    glm_vec2_copy(pos, mouse_last_position);

    if (cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
      glm_vec2_scale(direction, zoom / 100.F, direction);
      glm_vec2_add(camera->position, direction, camera->position);

      glm_mat4_identity(camera->view);
      glm_translate(camera->view, camera->position);
      camera->update = true;
    }
  }
}

static void ortho_key_callback(CmKeyEvent *event, CmScene *scene) {
  (void)scene;
  if (event->action == CM_KEY_PRESS) {
    event->base.handled = true;
    switch (event->code) {
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

static bool ortho_init(CmScene *scene, CmLayer *layer) {
  grid_shader = cm_load_shader_from_file("res/shader/basic.vs.glsl",
                                         "res/shader/basic.fs.glsl");

  cm_mouseinfo_pos(mouse_last_position);
  zoom = ORTHO_INITIAL_ZOOM;
  aspect = (float)scene->app->window->width / (float)scene->app->window->height;
  glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 100.F,
            layer->camera.projection);
  glm_mat4_identity(layer->camera.view);
  glm_vec3_copy(camera_initial_position, layer->camera.position);
  glm_translate(layer->camera.view, layer->camera.position);
  layer->camera.update = true;

  font = cm_font_init("res/fonts/Ubuntu.ttf", font_size);

  cm_event_set_callback(CM_EVENT_WINDOW_RESIZE,
                        (cm_event_callback)ortho_window_resize_callback,
                        &layer->camera);
  cm_event_set_callback(CM_EVENT_MOUSE, (cm_event_callback)ortho_mouse_callback,
                        &layer->camera);
  cm_event_set_callback(CM_EVENT_SCROLL,
                        (cm_event_callback)ortho_scroll_callback,
                        &layer->camera);

  cm_event_set_callback(CM_EVENT_KEYBOARD,
                        (cm_event_callback)ortho_key_callback, scene);

  cm_renderer_set_clear_color((vec4){0});

  glfwSwapInterval(0);
  return true;
}

static void ortho_update(CmScene *scene, CmLayer *layer, float dt) {
  (void)dt, (void)layer, (void)model, (void)scene;

  static mat4 mvp;
  glm_mat4_mul(layer->camera.vp, model, mvp);

  cm_shader_bind(&grid_shader);
  cm_shader_set_mat4(&grid_shader, "u_mvp", mvp);

  cm_renderer2d_begin();
  static uint32_t grid_size = 0; // 317^2 == 100'000 quads
  const float dt_min = 1 / 65.F;
  if (dt < dt_min) {
    grid_size += 1;
  }
  const float quad_size = 5.F;
  static float rotation = 0.F;
  const float rotation_speed = 45.F;
  rotation += rotation_speed * dt;
  for (size_t i = 0; i < grid_size; i++) {
    for (size_t j = 0; j < grid_size; j++) {
      cm_renderer2d_push_quad_color_rotated(
          (vec2){
              i * (quad_size + quad_size / 2),
              j * (quad_size + quad_size / 2),
          },
          0.F, (vec2){quad_size, quad_size}, (vec4){0.F, 0.F, 1.F, 1.F},
          glm_rad(rotation + i + j));
    }
  }
  cm_renderer2d_end();

  cm_shader_unbind();

#define LABEL_SIZE 128
  char label_buffer[LABEL_SIZE];
  const size_t len =
      snprintf(label_buffer, LABEL_SIZE - 1, "Batch renderer: %u quads",
               grid_size * grid_size);
  cm_font_draw(font, mvp, 0.F, -100.F, 1.F, len, label_buffer);
}

static void ortho_free(CmScene *scene, CmLayer *layer) {
  (void)layer, (void)scene;
  cm_shader_delete(&grid_shader);
  cm_font_free(font);
}

CmLayerInterface sandbox_ortho(void) {
  return (CmLayerInterface){
      .init = ortho_init,
      .free = ortho_free,
      .update = ortho_update,
  };
}
