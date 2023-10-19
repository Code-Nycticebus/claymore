#include "instancing.h"
#include <stdlib.h>

#include "mesh.h"

typedef struct {
  vec3 pos;
  vec4s color;
  vec3 normal;
} Vertex;

CmLayerInterface sandbox_fps(void);

static CmFont *font;
static const float font_size = 24.F;

static const float fov = 90.F;

size_t transform_count;
typedef struct Transform {
  vec4s color;
  mat4s transform;
} Transform;

CmRenderBuffer buffer;
CmRenderBuffer buffer2;

#define INSTANCED_DYNAMIC_CUBES 100000

static CmShader cube_shader;
CmMesh *cube_dynamic_mesh;

struct {
  vec3s pos;
  vec4s color;
} light;
static CmShader light_shader;
CmMesh *cube_static_mesh;

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
  const float min_distance = 1.F;
  const float max_distance = 1000.F;

  vec3s direction;
  vec3s center = {{0, 0, 0}};
  float distance = glms_vec3_distance(center, camera->position);
  direction = glms_vec3_sub(center, camera->position);
  direction = glms_vec3_normalize(direction);
  direction = glms_vec3_scale(direction, (float)event->yoffset * distance / 4);

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

static bool instancing_scene_init(CmScene *scene) {
  (void)scene;
  scene->camera = cm_camera_init_perspective(
      (vec3s){{0, 0, 4}}, (vec3s){0}, fov,
      (float)scene->app->window->width / (float)scene->app->window->height);
  cube_shader = cm_shader_load_from_file("res/shader/cube.vs.glsl",
                                         "res/shader/cube.fs.glsl");
  light_shader = cm_shader_load_from_file("res/shader/cube.vs.glsl",
                                          "res/shader/basic_instance.fs.glsl");
  Vertex cube_vertices[] = {
      // Front
      {{0.F, 0.F, 0.F}, {{1, 1, 1, 1}}, {0, 0, 1}},
      {{0.F, 1.F, 0.F}, {{1, 1, 1, 1}}, {0, 0, 1}},
      {{1.F, 1.F, 0.F}, {{1, 1, 1, 1}}, {0, 0, 1}},
      {{1.F, 0.F, 0.F}, {{1, 1, 1, 1}}, {0, 0, 1}},

      // Right
      {{1.F, 0.F, 0.F}, {{1, 1, 1, 1}}, {1, 0, 0}},
      {{1.F, 1.F, 0.F}, {{1, 1, 1, 1}}, {1, 0, 0}},
      {{1.F, 1.F, -1.F}, {{1, 1, 1, 1}}, {1, 0, 0}},
      {{1.F, 0.F, -1.F}, {{1, 1, 1, 1}}, {1, 0, 0}},

      // Left
      {{0.F, 0.F, 0.F}, {{1, 1, 1, 1}}, {-1, 0, 0}},
      {{0.F, 0.F, -1.F}, {{1, 1, 1, 1}}, {-1, 0, 0}},
      {{0.F, 1.F, -1.F}, {{1, 1, 1, 1}}, {-1, 0, 0}},
      {{0.F, 1.F, 0.F}, {{1, 1, 1, 1}}, {-1, 0, 0}},

      // Back
      {{0.F, 0.F, -1.F}, {{1, 1, 1, 1}}, {0, 0, -1}},
      {{1.F, 0.F, -1.F}, {{1, 1, 1, 1}}, {0, 0, -1}},
      {{1.F, 1.F, -1.F}, {{1, 1, 1, 1}}, {0, 0, -1}},
      {{0.F, 1.F, -1.F}, {{1, 1, 1, 1}}, {0, 0, -1}},

      // Top
      {{1.F, 1.F, 0.F}, {{1, 1, 1, 1}}, {0, 1, 0}},
      {{0.F, 1.F, 0.F}, {{1, 1, 1, 1}}, {0, 1, 0}},
      {{0.F, 1.F, -1.F}, {{1, 1, 1, 1}}, {0, 1, 0}},
      {{1.F, 1.F, -1.F}, {{1, 1, 1, 1}}, {0, 1, 0}},

      // Bottom
      {{1.F, 0.F, 0.F}, {{1, 1, 1, 1}}, {0, -1, 0}},
      {{1.F, 0.F, -1.F}, {{1, 1, 1, 1}}, {0, -1, 0}},
      {{0.F, 0.F, -1.F}, {{1, 1, 1, 1}}, {0, -1, 0}},
      {{0.F, 0.F, 0.F}, {{1, 1, 1, 1}}, {0, -1, 0}},
  };
  const size_t vertices_count =
      sizeof(cube_vertices) / sizeof(cube_vertices[0]);

  const uint32_t cube_indices[] = {
      0,  1,  2,  0,  2,  3,  // Front
      4,  5,  6,  4,  6,  7,  // Right
      8,  9,  10, 8,  10, 11, // Left
      12, 13, 14, 12, 14, 15, // Back
      16, 17, 18, 16, 18, 19, // Top
      20, 21, 22, 20, 22, 23, // Bottom
  };
  const size_t indices_count = sizeof(cube_indices) / sizeof(cube_indices[0]);

  buffer.vertex_buffer = cm_vertex_buffer_create(
      vertices_count, sizeof(cube_vertices[0]), cube_vertices, GL_DYNAMIC_DRAW);
  buffer.vertex_attribute = cm_vertex_attribute_create(&buffer.vertex_buffer);
  cm_vertex_attribute_push(&buffer.vertex_attribute, 3, GL_FLOAT,
                           offsetof(Vertex, pos));
  cm_vertex_attribute_push(&buffer.vertex_attribute, 4, GL_FLOAT,
                           offsetof(Vertex, color));
  cm_vertex_attribute_push(&buffer.vertex_attribute, 3, GL_FLOAT,
                           offsetof(Vertex, normal));
  buffer.index_buffer = cm_index_buffer_create(
      &buffer.vertex_attribute, indices_count, cube_indices, GL_DYNAMIC_DRAW);

  buffer2.vertex_buffer = cm_vertex_buffer_create(
      vertices_count, sizeof(cube_vertices[0]), cube_vertices, GL_DYNAMIC_DRAW);
  buffer2.vertex_attribute = cm_vertex_attribute_create(&buffer2.vertex_buffer);
  cm_vertex_attribute_push(&buffer2.vertex_attribute, 3, GL_FLOAT,
                           offsetof(Vertex, pos));
  cm_vertex_attribute_push(&buffer2.vertex_attribute, 4, GL_FLOAT,
                           offsetof(Vertex, color));
  cm_vertex_attribute_push(&buffer2.vertex_attribute, 3, GL_FLOAT,
                           offsetof(Vertex, normal));

  buffer2.index_buffer = cm_index_buffer_create(
      &buffer2.vertex_attribute, indices_count, cube_indices, GL_DYNAMIC_DRAW);

  cube_dynamic_mesh = cm_mesh_create(&buffer, false, INSTANCED_DYNAMIC_CUBES);
  cube_static_mesh = cm_mesh_create(&buffer2, true, 1);

  light.pos = (vec3s){{-4, 4, 4}};
  light.color = (vec4s){{1, 1, 1, 1}};
  cm_mesh_transform_push(cube_static_mesh, glms_translate_make(light.pos));

  cm_event_subscribe(CM_EVENT_WINDOW_RESIZE, (cm_event_callback)camera_resize,
                     &scene->camera);

  cm_event_subscribe(CM_EVENT_KEYBOARD, (cm_event_callback)cube_key_callback,
                     scene);

  cm_event_subscribe(CM_EVENT_MOUSE, (cm_event_callback)camera_controll,
                     &scene->camera);
  cm_event_subscribe(CM_EVENT_SCROLL, (cm_event_callback)camera_scroll,
                     &scene->camera);
  cm_event_subscribe(CM_EVENT_WINDOW_RESIZE, (cm_event_callback)camera_resize,
                     &scene->camera);

  font = cm_font_init("res/fonts/Ubuntu.ttf", font_size);
  glfwSwapInterval(0);
  return true;
}

static void instancing_scene_update(CmScene *scene, float dt) {
  (void)scene, (void)dt;
  mat4s model = glms_mat4_identity();

  cm_shader_bind(&light_shader);
  cm_shader_set_mat4(&light_shader, "u_vp", scene->camera.vp);
  cm_shader_set_mat4(&light_shader, "u_model", model);

  cm_mesh_draw(cube_static_mesh);

  cm_shader_unbind();

  cm_shader_bind(&cube_shader);
  cm_shader_set_mat4(&cube_shader, "u_vp", scene->camera.vp);
  cm_shader_set_mat4(&cube_shader, "u_model", model);

  cm_shader_set_vec3(&cube_shader, "u_light_pos", light.pos);
  cm_shader_set_vec4(&cube_shader, "u_light_color", light.color);
  cm_shader_set_vec3(&cube_shader, "u_view_pos", scene->camera.position);

  static size_t grid_size = 3;
  assert(grid_size < 1000 && "grid to big");
  const float dt_max = 1 / 65.F;
  static float rotation = 0;
  const vec3s size = (vec3s){{.5F, .5F, .5F}};
  rotation += 4 * dt;
  grid_size += dt < dt_max ? +1 : 0;
  for (size_t x = 0; x < grid_size; x++) {
    for (size_t y = 0; y < grid_size; y++) {
      for (size_t z = 0; z < grid_size; z++) {
        mat4s trans = glms_translate_make((vec3s){{x, y, z}});
        mat4s rot = glms_rotate_make(glm_rad(rotation), (vec3s){{1, 1, 1}});
        mat4s scale = glms_scale_make(size);
        trans = glms_mat4_mul(trans, rot);
        trans = glms_mat4_mul(trans, scale);
        cm_mesh_transform_push(cube_dynamic_mesh, trans);
      }
    }
  }
  cm_mesh_draw(cube_dynamic_mesh);
  cm_mesh_transform_clear(cube_dynamic_mesh);

  cm_shader_unbind();

#define LABEL_SIZE 128
  char label_buffer[LABEL_SIZE];
  const size_t len = snprintf(label_buffer, LABEL_SIZE - 1, "%lu cubes",
                              grid_size * grid_size * (uint64_t)grid_size);
  mat4s mvp = glms_mat4_mul(model, scene->camera.vp);
  cm_font_draw(font, mvp, 0.F, -font_size, -100.F, len, label_buffer);
}

static void instancing_scene_free(CmScene *scene) {
  (void)scene;
  cm_mesh_delete(cube_static_mesh);
  cm_mesh_delete(cube_dynamic_mesh);
  cm_render_buffer_delete(&buffer);
}

CmSceneInterface scene_instancing(void) {
  return (CmSceneInterface){
      .init = instancing_scene_init,
      .update = instancing_scene_update,
      .free = instancing_scene_free,
      .layers = {sandbox_fps, NULL},
  };
}
