#include "claymore.h"

static CmShader cube_shader;

static vec2s mouse_last_pos = {{0, 0}};
static CmRenderBuffer render_data;

static GLenum draw_mode = GL_FILL;

const float rotation = 90.F;
static mat4s model = GLMS_MAT4_IDENTITY_INIT;

struct Vertex {
  vec3 pos;
  vec4 color;
};

static const float fov = 45.F;

static void camera_controll(CmMouseEvent *event, CmCamera *camera) {
  if (event->action == CM_MOUSE_MOVE) {
    vec2s mouse_pos = cm_mouseinfo_pos();

    vec2s dir = glms_vec2_sub(mouse_pos, mouse_last_pos);
    glms_vec2_normalize(dir);
    dir = glms_vec2_scale(dir, (float)3);

    mouse_last_pos = mouse_pos;

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
  vec3s direction;
  vec3s center = {{0, 0, 0}};
  const float min_distance = 0.1F;
  const float max_distance = 100.F;

  direction = glms_vec3_sub(center, camera->position);
  direction = glms_vec3_normalize(direction);
  direction = glms_vec3_scale(direction, (float)event->yoffset);

  vec3s new_camera_pos;
  new_camera_pos = glms_vec3_add(camera->position, direction);
  float new_distance = glms_vec3_distance(new_camera_pos, center);
  if (min_distance <= new_distance && new_distance <= max_distance) {
    cm_camera_position(camera, new_camera_pos);
  }
}

static void camera_resize(CmWindowEvent *event, CmCamera *camera) {
  (void)event;
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
  cm_event_set_callback(CM_EVENT_KEYBOARD, (cm_event_callback)cube_key_callback,
                        layer);

  cm_event_set_callback(CM_EVENT_MOUSE, (cm_event_callback)camera_controll,
                        &layer->camera);
  cm_event_set_callback(CM_EVENT_SCROLL, (cm_event_callback)camera_scroll,
                        &layer->camera);
  cm_event_set_callback(CM_EVENT_WINDOW_RESIZE,
                        (cm_event_callback)camera_resize, &layer->camera);

  cube_shader = cm_shader_load_from_file("res/shader/basic.vs.glsl",
                                         "res/shader/basic.fs.glsl");

  layer->camera = cm_camera_init_perspective(
      (vec3s){{0, 0, 4}}, (vec3s){0}, fov,
      (float)scene->app->window->width / (float)scene->app->window->height);

  struct Vertex cube_vertices[] = {
      // 1
      {{-1.F, -1.F, 1.F}, {1.F, 1.F, 0.F, 1.F}},
      {{1.F, -1.F, 1.F}, {1.F, 0.F, 0.F, 1.F}},
      {{1.F, 1.F, 1.F}, {0.F, 1.F, 1.F, 1.F}},
      {{-1.F, 1.F, 1.F}, {1.F, 0.F, 0.F, 1.F}},

      // 2
      {{-1.F, -1.F, -1.F}, {0.F, 0.F, 1.F, 1.F}},
      {{1.F, -1.F, -1.F}, {0.F, 1.F, 1.F, 1.F}},
      {{1.F, 1.F, -1.F}, {1.F, 0.F, 1.F, 1.F}},
      {{-1.F, 1.F, -1.F}, {1.F, 1.F, 0.F, 1.F}},
  };
  const size_t vertices_count = 8;

  render_data.vertex_buffer = cm_vertex_buffer_create(
      vertices_count, sizeof(struct Vertex), cube_vertices, GL_STATIC_DRAW);

  render_data.vertex_attribute =
      cm_vertex_attribute_create(&render_data.vertex_buffer);
  cm_vertex_attribute_push(&render_data.vertex_attribute, 3, GL_FLOAT,
                           offsetof(struct Vertex, pos));
  cm_vertex_attribute_push(&render_data.vertex_attribute, 4, GL_FLOAT,
                           offsetof(struct Vertex, color));

  const uint32_t cube_indices[] = {
      0, 1, 2, 0, 2, 3, // 1
      4, 5, 6, 4, 6, 7, // 2
      0, 4, 5, 0, 5, 1, // 3
      0, 4, 7, 0, 3, 7, // 4
      1, 2, 5, 5, 6, 2, // 5
      2, 3, 6, 3, 6, 7, // 6
  };
  const size_t indices_count = sizeof(cube_indices) / sizeof(cube_indices[0]);

  render_data.index_buffer =
      cm_index_buffer_create(&render_data.vertex_attribute, indices_count,
                             cube_indices, GL_STATIC_DRAW);
  cm_renderer_set_clear_color((vec4s){{0.F, 0.F, 0.F, 1.F}});
  mouse_last_pos = cm_mouseinfo_pos();
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

  cm_renderer_draw_indexed(&render_data, render_data.index_buffer.count);

  cm_shader_unbind();

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Reset to normal mode
}

static void cube_free(CmScene *scene, CmLayer *layer) {
  (void)layer, (void)scene;
  cm_render_buffer_delete(&render_data);
  cm_shader_delete(&cube_shader);
}

CmLayerInterface sandbox_cube(void) {
  return (const CmLayerInterface){
      .init = cube_init,
      .free = cube_free,
      .update = cube_update,
  };
}