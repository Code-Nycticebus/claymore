#include "instancing.h"
#include <stdlib.h>

#include "mesh.h"

CmLayerInterface sandbox_fps(void);

static CmFont *font;
static const float font_size = 24.F;

static const float fov = 90.F;

#define GRID_SIZE 60

static CmShader cube_shader;

struct {
  vec3s pos;
  vec4s color;
} light;

CmMesh cube_mesh;

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
  vec3s vertex_positions[] = {
      // Front
      {{0.F, 0.F, 0.F}},
      {{0.F, 1.F, 0.F}},
      {{1.F, 1.F, 0.F}},
      {{1.F, 0.F, 0.F}},

      // Right
      {{1.F, 0.F, 0.F}},
      {{1.F, 1.F, 0.F}},
      {{1.F, 1.F, -1.F}},
      {{1.F, 0.F, -1.F}},

      // Left
      {{0.F, 0.F, 0.F}},
      {{0.F, 0.F, -1.F}},
      {{0.F, 1.F, -1.F}},
      {{0.F, 1.F, 0.F}},

      // Back
      {{0.F, 0.F, -1.F}},
      {{1.F, 0.F, -1.F}},
      {{1.F, 1.F, -1.F}},
      {{0.F, 1.F, -1.F}},

      // Top
      {{1.F, 1.F, 0.F}},
      {{0.F, 1.F, 0.F}},
      {{0.F, 1.F, -1.F}},
      {{1.F, 1.F, -1.F}},

      // Bottom
      {{1.F, 0.F, 0.F}},
      {{1.F, 0.F, -1.F}},
      {{0.F, 0.F, -1.F}},
      {{0.F, 0.F, 0.F}},
  };
  vec4s vertex_colors[] = {
      // Front
      {{1, 0, 0, 1}},
      {{1, 0, 0, 1}},
      {{1, 0, 0, 1}},
      {{1, 0, 0, 1}},

      // Right
      {{1, 1, 0, 1}},
      {{1, 1, 0, 1}},
      {{1, 1, 0, 1}},
      {{1, 1, 0, 1}},

      // Left
      {{0, 1, 1, 1}},
      {{0, 1, 1, 1}},
      {{0, 1, 1, 1}},
      {{0, 1, 1, 1}},

      // Back
      {{0, 0, 1, 1}},
      {{0, 0, 1, 1}},
      {{0, 0, 1, 1}},
      {{0, 0, 1, 1}},

      // Top
      {{1, 0, 1, 1}},
      {{1, 0, 1, 1}},
      {{1, 0, 1, 1}},
      {{1, 0, 1, 1}},

      // Bottom
      {{0, 1, 0, 1}},
      {{0, 1, 0, 1}},
      {{0, 1, 0, 1}},
      {{0, 1, 0, 1}},
  };

  const size_t vertices_count =
      sizeof(vertex_positions) / sizeof(vertex_positions[0]);

  const uint32_t cube_indices[] = {
      0,  1,  2,  0,  2,  3,  // Front
      4,  5,  6,  4,  6,  7,  // Right
      8,  9,  10, 8,  10, 11, // Left
      12, 13, 14, 12, 14, 15, // Back
      16, 17, 18, 16, 18, 19, // Top
      20, 21, 22, 20, 22, 23, // Bottom
  };
  const size_t indices_count = sizeof(cube_indices) / sizeof(cube_indices[0]);

  cube_mesh = cm_mesh_create(cube_indices, indices_count);
  cm_mesh_push_positions(&cube_mesh, vertex_positions, vertices_count);
  cm_mesh_push_colors(&cube_mesh, vertex_colors, vertices_count);
  mat4s *transforms = malloc(sizeof(mat4s) * GRID_SIZE * GRID_SIZE * GRID_SIZE);
  size_t transform_count = 0;
  static float r = 0;
  const vec3s axis = {{1, 1, 1}};
  const vec3s scale = {{.5F, .5F, .5F}};
  for (size_t x = 0; x < GRID_SIZE; x++) {
    for (size_t y = 0; y < GRID_SIZE; y++) {
      for (size_t z = 0; z < GRID_SIZE; z++) {
        r += 1;
        mat4s trans_mat = glms_translate_make((vec3s){{x, y, z}});
        mat4s rot_mat = glms_rotate_make(glm_rad(r), axis);
        mat4s scale_mat = glms_scale_make(scale);
        transforms[transform_count] = glms_mat4_mul(trans_mat, rot_mat);
        transforms[transform_count] =
            glms_mat4_mul(transforms[transform_count], scale_mat);
        transform_count++;
      }
    }
  }
  cm_mesh_push_transforms(&cube_mesh, transforms, transform_count);
  free(transforms);

  light.pos = (vec3s){{-4, 4, 4}};
  light.color = (vec4s){{1, 1, 1, 1}};

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

  cm_shader_bind(&cube_shader);
  cm_shader_set_mat4(&cube_shader, "u_vp", scene->camera.vp);

  // RENDER HERE
  cm_mesh_draw(&cube_mesh);

  cm_shader_unbind();
}

static void instancing_scene_free(CmScene *scene) { (void)scene; }

CmSceneInterface scene_instancing(void) {
  return (CmSceneInterface){
      .init = instancing_scene_init,
      .update = instancing_scene_update,
      .free = instancing_scene_free,
      .layers = {sandbox_fps, NULL},
  };
}
