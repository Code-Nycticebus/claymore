#include "instancing.h"
#include <stdlib.h>

CmLayerInterface sandbox_fps(void);

struct Vertex {
  vec3 pos;
  vec4 color;
  vec3 normal;
};

static CmShader cube_shader;
static CmRenderBuffer render_data_cube;
static CmVertexBuffer vbo;

static const float fov = 60.F;

size_t transform_count;
mat4s *transform;

#define INSTANCED_CUBES 60000

static struct {
  vec3s position;
  vec3s size;
  float rotation;
  vec3s axis;
} Cubes[INSTANCED_CUBES];

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
  const float max_distance = 1000.F;

  vec3s direction;
  vec3s center = {{0, 0, 0}};
  direction = glms_vec3_sub(center, camera->position);
  direction = glms_vec3_normalize(direction);
  direction = glms_vec3_scale(direction, (float)event->yoffset * 100.F);

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

  struct Vertex cube_vertices[] = {
      // Front
      {{-1.F, -1.F, 1.F}, {1.F, 0.F, 0.F, 1.F}, {0, 0, 1}},
      {{-1.F, 1.F, 1.F}, {1.F, 0.F, 0.F, 1.F}, {0, 0, 1}},
      {{1.F, 1.F, 1.F}, {1.F, 0.F, 0.F, 1.F}, {0, 0, 1}},
      {{1.F, -1.F, 1.F}, {1.F, 0.F, 0.F, 1.F}, {0, 0, 1}},

      // Right
      {{1.F, -1.F, 1.F}, {0.F, 1.F, 0.F, 1.F}, {1, 0, 0}},
      {{1.F, 1.F, 1.F}, {0.F, 1.F, 0.F, 1.F}, {1, 0, 0}},
      {{1.F, 1.F, -1.F}, {0.F, 1.F, 0.F, 1.F}, {1, 0, 0}},
      {{1.F, -1.F, -1.F}, {0.F, 1.F, 0.F, 1.F}, {1, 0, 0}},

      // Left
      {{-1.F, -1.F, 1.F}, {0.F, 1.F, 0.F, 1.F}, {-1, 0, 0}},
      {{-1.F, -1.F, -1.F}, {0.F, 1.F, 0.F, 1.F}, {-1, 0, 0}},
      {{-1.F, 1.F, -1.F}, {0.F, 1.F, 0.F, 1.F}, {-1, 0, 0}},
      {{-1.F, 1.F, 1.F}, {0.F, 1.F, 0.F, 1.F}, {-1, 0, 0}},

      // Back
      {{-1.F, -1.F, -1.F}, {1.F, 0.F, 0.F, 1.F}, {0, 0, -1}},
      {{1.F, -1.F, -1.F}, {1.F, 0.F, 0.F, 1.F}, {0, 0, -1}},
      {{1.F, 1.F, -1.F}, {1.F, 0.F, 0.F, 1.F}, {0, 0, -1}},
      {{-1.F, 1.F, -1.F}, {1.F, 0.F, 0.F, 1.F}, {0, 0, -1}},

      // Top
      {{1.F, 1.F, 1.F}, {0.F, 0.F, 1.F, 1.F}, {0, 1, 0}},
      {{-1.F, 1.F, 1.F}, {0.F, 0.F, 1.F, 1.F}, {0, 1, 0}},
      {{-1.F, 1.F, -1.F}, {0.F, 0.F, 1.F, 1.F}, {0, 1, 0}},
      {{1.F, 1.F, -1.F}, {0.F, 0.F, 1.F, 1.F}, {0, 1, 0}},

      // Bottom
      {{1.F, -1.F, 1.F}, {0.F, 0.F, 1.F, 1.F}, {0, -1, 0}},
      {{1.F, -1.F, -1.F}, {0.F, 0.F, 1.F, 1.F}, {0, -1, 0}},
      {{-1.F, -1.F, -1.F}, {0.F, 0.F, 1.F, 1.F}, {0, -1, 0}},
      {{-1.F, -1.F, 1.F}, {0.F, 0.F, 1.F, 1.F}, {0, -1, 0}},

  };
  const size_t vertices_count =
      sizeof(cube_vertices) / sizeof(cube_vertices[0]);

  render_data_cube.vertex_buffer = cm_vertex_buffer_create(
      vertices_count, sizeof(struct Vertex), cube_vertices, GL_STATIC_DRAW);
  render_data_cube.vertex_attribute =
      cm_vertex_attribute_create(&render_data_cube.vertex_buffer);
  cm_vertex_attribute_push(&render_data_cube.vertex_attribute, 3, GL_FLOAT,
                           offsetof(struct Vertex, pos));
  cm_vertex_attribute_push(&render_data_cube.vertex_attribute, 4, GL_FLOAT,
                           offsetof(struct Vertex, color));
  cm_vertex_attribute_push(&render_data_cube.vertex_attribute, 3, GL_FLOAT,
                           offsetof(struct Vertex, normal));

  const uint32_t cube_indices[] = {
      0,  1,  2,  0,  2,  3,  // Front
      4,  5,  6,  4,  6,  7,  // Right
      8,  9,  10, 8,  10, 11, // Left
      12, 13, 14, 12, 14, 15, // Back
      16, 17, 18, 16, 18, 19, // Top
      20, 21, 22, 20, 22, 23, // Bottom
  };
  const size_t indices_count = sizeof(cube_indices) / sizeof(cube_indices[0]);
  render_data_cube.index_buffer =
      cm_index_buffer_create(&render_data_cube.vertex_attribute, indices_count,
                             cube_indices, GL_STATIC_DRAW);

  transform = malloc(sizeof(mat4s) * INSTANCED_CUBES);

  vbo = cm_vertex_buffer_create(1, sizeof(mat4s) * INSTANCED_CUBES, transform,
                                GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
  glBindVertexArray(render_data_cube.vertex_attribute.id);

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void *)0);
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(mat4),
                        (void *)(sizeof(vec4)));
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(mat4),
                        (void *)(2 * sizeof(vec4)));
  glEnableVertexAttribArray(6);
  glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(mat4),
                        (void *)(3 * sizeof(vec4)));

  glVertexAttribDivisor(3, 1);
  glVertexAttribDivisor(4, 1);
  glVertexAttribDivisor(5, 1);
  glVertexAttribDivisor(6, 1);

  for (size_t i = 0; i < INSTANCED_CUBES; ++i) {
    const float x = rand() % 200 - 100;
    const float y = rand() % 200 - 100;
    const float z = rand() % 200 - 100;
    const float s = rand() % 2 + 1;
    const float r = rand() % 360;
    vec3s axis = {{1, 1, 1}};

    Cubes[i].position = (vec3s){{x, y, z}};
    Cubes[i].size = (vec3s){{s, s, s}};
    Cubes[i].rotation = r;
    Cubes[i].axis = axis;
  }

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
  return true;
}

static void instancing_scene_update(CmScene *scene, float dt) {
  (void)scene, (void)dt;
  mat4s model = glms_mat4_identity();
  mat4s mvp = glms_mat4_mul(scene->camera.vp, model);

  glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(mat4s) * INSTANCED_CUBES,
                  transform);

  cm_shader_bind(&cube_shader);
  cm_shader_set_mat4(&cube_shader, "u_mvp", mvp);
  glBindVertexArray(render_data_cube.vertex_attribute.id);
  for (size_t i = 0; i < INSTANCED_CUBES; ++i) {
    const float rotation_pers_second = 20;
    Cubes[i].rotation += rotation_pers_second * dt;
    mat4s scale = glms_scale_make(Cubes[i].size);
    mat4s rot = glms_rotate_make(glm_rad(Cubes[i].rotation), Cubes[i].axis);
    mat4s trans = glms_translate_make(Cubes[i].position);

    transform[i] = glms_mat4_mul(scale, trans);
    transform[i] = glms_mat4_mul(transform[i], rot);
  }
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_data_cube.index_buffer.id);

  glDrawElementsInstanced(GL_TRIANGLES, render_data_cube.index_buffer.count,
                          GL_UNSIGNED_INT, NULL, INSTANCED_CUBES);

  cm_shader_unbind();
  (void)mvp;
}

static void instancing_scene_free(CmScene *scene) {
  (void)scene;
  free(transform);
}

CmSceneInterface scene_instancing(void) {
  return (CmSceneInterface){
      .init = instancing_scene_init,
      .update = instancing_scene_update,
      .free = instancing_scene_free,
      .layers = {sandbox_fps, NULL},
  };
}
