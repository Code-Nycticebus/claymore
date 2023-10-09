#include "claymore.h"

static CmShader cube_shader;
static CmShader light_shader;

static CmRenderBuffer render_data_light;

static GLenum draw_mode = GL_FILL;

static mat4s model = GLMS_MAT4_IDENTITY_INIT;

struct Vertex {
  vec3 pos;
  vec4 color;
  vec3 normal;
};

static const float fov = 60.F;

static CmFont *font;
static const float font_size = 24.F;

static void camera_controll(CmMouseEvent *event, CmCamera *camera) {
  if (event->action == CM_MOUSE_MOVE) {
    vec2s dir = cm_mouseinfo_direction();
    glms_vec2_normalize(dir);
    dir = glms_vec2_scale(dir, (float)3);

    if (cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
      float camera_distance =
          glms_vec3_distance(camera->position, camera->lookat);

      static float rotation_horizontal = 0.F;
      static float rotation_vertical = 0.F;

      rotation_horizontal += glm_rad(-dir.x);
      rotation_vertical += glm_rad(-dir.y);

      const float limit = glm_rad(89.F);
      rotation_vertical = glm_clamp(rotation_vertical, -limit, limit);

      vec3s new_camera_pos;
      new_camera_pos.x = camera->lookat.x + camera_distance *
                                                sinf(rotation_horizontal) *
                                                cosf(rotation_vertical);
      new_camera_pos.y =
          camera->lookat.y + camera_distance * sinf(rotation_vertical);
      new_camera_pos.z = camera->lookat.z + camera_distance *
                                                cosf(rotation_horizontal) *
                                                cosf(rotation_vertical);

      cm_camera_position(camera, new_camera_pos);
    }
  }
}

static void camera_scroll(CmScrollEvent *event, CmCamera *camera) {
  const float min_distance = 0.1F;
  const float max_distance = 100.F;

  vec3s direction;
  vec3s center = {{0, 0, 0}};
  direction = glms_vec3_sub(center, camera->position);
  direction = glms_vec3_normalize(direction);
  direction = glms_vec3_scale(direction, (float)event->yoffset);

  vec3s new_camera_pos = glms_vec3_add(camera->position, direction);
  float new_distance = glms_vec3_distance(new_camera_pos, center);
  if (min_distance <= new_distance && new_distance <= max_distance) {
    cm_camera_position(camera, new_camera_pos);
  }
}

static void camera_resize(CmWindowEvent *event, CmCamera *camera) {
  camera->projection = glms_perspective(
      glm_rad(fov), (float)event->window->width / (float)event->window->height,
      1 / 100.F, 100.F);
  camera->update = true;
}

static void cube_key_callback(CmKeyEvent *event, CmLayer *layer) {
  if (event->action == CM_KEY_PRESS) {
    event->base.handled = true;
    switch (event->code) {
    case CM_KEY_F5: {
      cm_camera_position(&layer->camera, (vec3s){{0, 0, 4}});
      break;
    }
    case CM_KEY_F1: {
      draw_mode = draw_mode == GL_LINE ? GL_FILL : GL_LINE;
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

static bool cube_init(CmScene *scene, CmLayer *layer) {
  cm_event_subscribe(CM_EVENT_KEYBOARD, (cm_event_callback)cube_key_callback,
                     layer);

  cm_event_subscribe(CM_EVENT_MOUSE, (cm_event_callback)camera_controll,
                     &layer->camera);
  cm_event_subscribe(CM_EVENT_SCROLL, (cm_event_callback)camera_scroll,
                     &layer->camera);
  cm_event_subscribe(CM_EVENT_WINDOW_RESIZE, (cm_event_callback)camera_resize,
                     &layer->camera);

  cube_shader = cm_shader_load_from_file("res/shader/light.vs.glsl",
                                         "res/shader/light.fs.glsl");

  light_shader = cm_shader_load_from_file("res/shader/basic.vs.glsl",
                                          "res/shader/basic.fs.glsl");

  layer->camera = cm_camera_init_perspective(
      (vec3s){{0, 0, 4 * 4}}, (vec3s){0}, fov,
      (float)scene->app->window->width / (float)scene->app->window->height);

  cm_renderer_set_clear_color((vec4s){{0.F, 0.F, 0.F, 1.F}});

  font = cm_font_init("res/fonts/Ubuntu.ttf", font_size);

  glfwSwapInterval(0);
  return true;
}

static void cube_update(CmScene *scene, CmLayer *layer, float dt) {
  (void)scene;
  glPolygonMode(GL_FRONT_AND_BACK, draw_mode);
  static mat4s mvp;
  (void)dt;

  mvp = glms_mat4_mul(layer->camera.vp, model);

  cm_shader_bind(&cube_shader);
  cm_shader_set_mat4(&cube_shader, "u_mvp", mvp);
  cm_shader_set_mat4(&cube_shader, "u_model", model);
  cm_shader_set_vec3(&cube_shader, "u_light_pos", layer->camera.position);
  cm_shader_set_vec3(&cube_shader, "u_view_pos", layer->camera.position);

  cm_renderer3d_begin();
  static uint32_t grid_size = 3; // 317^2 == 100'000 quads
  const float dt_min = 1 / 70.F;
  grid_size += dt < dt_min ? +1 : 0;
  assert(grid_size < 1000 && "grid size is definitely too big");

  static float cube_rotation = 0;
  cube_rotation += 4 * dt;
  const vec3s cube_size = {{.6F, .6F, .6F}};
  const float color_min = 0.2F;
  const float color_max = 0.8F;
  for (size_t i = 0; i < grid_size; i++) {
    for (size_t j = 0; j < grid_size; j++) {
      for (size_t z = 0; z < grid_size; z++) {
        const vec3s color =
            glms_vec3_clamp((vec3s){{i / (float)grid_size, j / (float)grid_size,
                                     z / (float)grid_size}},
                            color_min, color_max);
        cm_renderer3d_push_cube_color_rotated(
            (vec3s){{i, j, z}}, cube_size,
            (vec4s){{color.r, color.g, color.b, 1}}, glm_rad(cube_rotation),
            (vec3s){{1, 1, 1}});
      }
    }
  }
  cm_renderer3d_end();

  cm_shader_unbind();

#define LABEL_SIZE 128
  char label_buffer[LABEL_SIZE];
  const size_t len = snprintf(label_buffer, LABEL_SIZE - 1, "%u cubes",
                              grid_size * grid_size * grid_size);
  cm_font_draw(font, mvp, 0.F, -font_size, -100.F, len, label_buffer);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Reset to normal mode
}

static void cube_free(CmScene *scene, CmLayer *layer) {
  (void)layer, (void)scene;
  cm_render_buffer_delete(&render_data_light);
  cm_shader_delete(&cube_shader);
  cm_shader_delete(&light_shader);
}

CmLayerInterface sandbox_cube_bench(void) {
  return (const CmLayerInterface){
      .init = cube_init,
      .free = cube_free,
      .update = cube_update,
  };
}
