#include "claymore.h"

struct Vertex {
  vec3 pos;
  vec2 uv;
};

static CmShader grid_shader;

static CmFont *font;
static const float font_size = 64.F;
static mat4s model = GLMS_MAT4_IDENTITY_INIT;

#define ORTHO_INITIAL_ZOOM 1400.F
static float zoom = ORTHO_INITIAL_ZOOM;
static vec3s camera_initial_position = {{0, 0, 0}};
static float aspect;
static vec2s mouse_last_position = {0};

static void ortho_scroll_callback(CmScrollEvent *event, CmCamera *camera) {
  const float min_zoom = 1.F;
  const float scroll_speed = 100.F;
  zoom = glm_max(zoom - event->yoffset * scroll_speed, min_zoom);
  camera->projection =
      glms_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 1.F);
  camera->update = true;
}

static void ortho_window_resize_callback(CmWindowEvent *event,
                                         CmCamera *camera) {
  aspect = (float)event->window->width / (float)event->window->height;
  camera->projection =
      glms_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 1.F);
  camera->update = true;
}

static void ortho_mouse_callback(CmMouseEvent *event, CmCamera *camera) {
  if (event->action == CM_MOUSE_MOVE) {

    vec2s pos = cm_mouseinfo_pos();
    vec2s direction = glms_vec2_sub(pos, mouse_last_position);
    mouse_last_position = pos;

    if (cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
      direction = glms_vec2_scale(direction, zoom / 100.F);
      camera->position = glms_vec3_add(camera->position,
                                       (vec3s){{direction.x, direction.y, 0}});

      camera->view = glms_mat4_identity();
      camera->view = glms_translate(camera->view, camera->position);
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
  grid_shader = cm_shader_load_from_file("res/shader/basic.vs.glsl",
                                         "res/shader/basic.fs.glsl");

  mouse_last_position = cm_mouseinfo_pos();
  zoom = ORTHO_INITIAL_ZOOM;
  aspect = (float)scene->app->window->width / (float)scene->app->window->height;
  layer->camera.projection =
      glms_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 100.F);
  layer->camera.view = glms_mat4_identity();
  layer->camera.position = camera_initial_position;
  layer->camera.view =
      glms_translate(layer->camera.view, layer->camera.position);
  layer->camera.update = true;
  printf("%f %f\n", layer->camera.position.x, layer->camera.position.y);

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

  cm_renderer_set_clear_color((vec4s){.r = 0.F, .g = 0.F, .b = 0.F, .a = 1.F});

  glfwSwapInterval(0);
  return true;
}

static void ortho_update(CmScene *scene, CmLayer *layer, float dt) {
  (void)dt, (void)layer, (void)scene;

  static mat4s mvp;
  mvp = glms_mat4_mul(layer->camera.vp, model);

  cm_shader_bind(&grid_shader);
  cm_shader_set_mat4(&grid_shader, "u_mvp", mvp);

  cm_renderer2d_begin();
  static uint32_t grid_size = 3; // 317^2 == 100'000 quads
  const float dt_min = 1 / 62.F;
  grid_size += dt < dt_min ? +1 : -1;
  assert(grid_size < 1000 && "grid size is definitely too big");

  const float quad_size = 5.F;
  static float rotation = 0.F;
  const float rotation_speed = 45.F;
  rotation += rotation_speed * dt;
  for (size_t i = 0; i < grid_size; i++) {
    for (size_t j = 0; j < grid_size; j++) {
      cm_renderer2d_push_quad_color_rotated(
          (vec2s){
              .x = i * (quad_size + quad_size / 2),
              .y = j * (quad_size + quad_size / 2),
          },
          0.F, (vec2s){{quad_size, quad_size}}, (vec4s){{0.F, 0.F, 1.F, 1.F}},
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
