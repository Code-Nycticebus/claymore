#include "claymore.h"

CmSceneInterface *sandbox(void);

typedef struct {

  CmMesh mesh;
  CmCamera3D camera;
  vec3 light_pos;
  CmShader shader;
  CmTexture2D texture[3];
} Test;

static void event(CmScene *scene, CmEvent *event) {
  Test *test = cm_scene_data(scene);
  CmCamera3D *camera = &test->camera;

  cm_event_cursor(event, {
    float camera_distance =
        glm_vec3_distance(camera->base.position, camera->lookat);

    static float rotation_horizontal = 0.F;
    static float rotation_vertical = 0.F;

    rotation_horizontal += glm_rad(-cursor->dir[0]);
    rotation_vertical += glm_rad(-cursor->dir[1]);

    const float limit = glm_rad(89.F);
    rotation_vertical = glm_clamp(rotation_vertical, -limit, limit);

    vec3 new_camera_pos;
    new_camera_pos[0] = camera->lookat[0] + camera_distance *
                                                sinf(rotation_horizontal) *
                                                cosf(rotation_vertical);
    new_camera_pos[1] =
        camera->lookat[1] + camera_distance * sinf(rotation_vertical);
    new_camera_pos[2] = camera->lookat[2] + camera_distance *
                                                cosf(rotation_horizontal) *
                                                cosf(rotation_vertical);

    cm_camera3D_position(camera, new_camera_pos);
  });
  cm_event_scroll(event, {
    const float min_distance = 1.F;
    const float max_distance = 1000.F;

    vec3 direction;
    vec3 center = {0, 0, 0};
    float distance = glm_vec3_distance(center, camera->base.position);
    glm_vec3_sub(center, camera->base.position, direction);
    glm_vec3_normalize(direction);
    glm_vec3_scale(direction, scroll->offset[1] * distance / 4, direction);

    vec3 new_camera_pos;
    glm_vec3_add(camera->base.position, direction, new_camera_pos);
    float new_distance = glm_vec3_distance(new_camera_pos, center);
    if (min_distance <= new_distance && new_distance <= max_distance) {
      cm_camera3D_position(camera, new_camera_pos);
    }
  });
}

static void init(CmScene *scene) {
  Test *test = cm_scene_data(scene);

  static const vec3 data[] = {
      // Front
      {0.F, 0.F, 0.F},
      {1.F, 0.F, 0.F},
      {1.F, 1.F, 0.F},
      {0.F, 1.F, 0.F},

      // Right
      {1.F, 0.F, 0.F},
      {1.F, 0.F, -1.F},
      {1.F, 1.F, -1.F},
      {1.F, 1.F, 0.F},

      // Left
      {0.F, 0.F, 0.F},
      {0.F, 1.F, 0.F},
      {0.F, 1.F, -1.F},
      {0.F, 0.F, -1.F},

      // Back
      {0.F, 0.F, -1.F},
      {0.F, 1.F, -1.F},
      {1.F, 1.F, -1.F},
      {1.F, 0.F, -1.F},

      // Top
      {1.F, 1.F, 0.F},
      {1.F, 1.F, -1.F},
      {0.F, 1.F, -1.F},
      {0.F, 1.F, 0.F},

      // Bottom
      {1.F, 0.F, 0.F},
      {0.F, 0.F, 0.F},
      {0.F, 0.F, -1.F},
      {1.F, 0.F, -1.F},
  };
  test->mesh = cm_mesh_create(&scene->gpu, ARRAY_LEN(data), data);

  const uint32_t cube_indices[] = {
      0,  1,  2,  0,  2,  3,  // Front
      4,  5,  6,  4,  6,  7,  // Right
      8,  9,  10, 8,  10, 11, // Left
      12, 13, 14, 12, 14, 15, // Back
      16, 17, 18, 16, 18, 19, // Top
      20, 21, 22, 20, 22, 23, // Bottom
  };
  cm_mesh_attach_ebo(&test->mesh, ARRAY_LEN(cube_indices), cube_indices);

  const vec3 normals[] = {
      // Front
      {0, 0, 1},
      {0, 0, 1},
      {0, 0, 1},
      {0, 0, 1},

      // Right
      {1, 0, 0},
      {1, 0, 0},
      {1, 0, 0},
      {1, 0, 0},

      // Left
      {-1, 0, 0},
      {-1, 0, 0},
      {-1, 0, 0},
      {-1, 0, 0},

      // Back
      {0, 0, -1},
      {0, 0, -1},
      {0, 0, -1},
      {0, 0, -1},

      // Top
      {0, 1, 0},
      {0, 1, 0},
      {0, 1, 0},
      {0, 1, 0},

      // Bottom
      {0, -1, 0},
      {0, -1, 0},
      {0, -1, 0},
      {0, -1, 0},
  };
  cm_mesh_attach_vec3(&test->mesh, ARRAY_LEN(normals), normals);

  const vec2 uv[] = {
      // Front
      {0, 0},
      {1, 0},
      {1, 1},
      {0, 1},

      // Right
      {0, 0},
      {1, 0},
      {1, 1},
      {0, 1},

      // Left
      {0, 0},
      {1, 0},
      {1, 1},
      {0, 1},

      // Back
      {0, 0},
      {0, 1},
      {1, 1},
      {1, 0},

      // Top
      {0, 0},
      {0, 1},
      {1, 1},
      {1, 0},

      // Bottom
      {0, 0},
      {0, 1},
      {1, 1},
      {1, 0},
  };
  cm_mesh_attach_vec2(&test->mesh, ARRAY_LEN(uv), uv);

  mat4 transforms[] = {
      GLM_MAT4_IDENTITY_INIT,
  };
  mat4 trans;
  glm_translate_make(trans, (vec3){-.5f, -.5f, .5f});
  mat4 rot;
  glm_rotate_make(rot, glm_rad(0), (vec3){0, 1, 0});
  mat4 scale;
  glm_scale_make(scale, (vec3){1, 1, 1});

  glm_mul(trans, rot, transforms[0]);
  glm_mul(transforms[0], scale, transforms[0]);

  cm_mesh_attach_mat4_instanced(&test->mesh, ARRAY_LEN(transforms), transforms);

  RGFW_window *window = cm_app_window();
  cm_camera3D_perspective(&test->camera, (vec3){2, 2, 2}, 80.f, (vec3){0, 0, 0},
                          window->r.w / (float)window->r.h);

  test->shader = cm_shader_from_memory(
      &scene->gpu,
      STR("#version 430 core\n"
          "layout (location = 0) in vec3 a_pos;\n"
          "layout (location = 1) in vec3 a_normals;\n"
          "layout (location = 2) in vec2 a_uv;\n"
          "layout (location = 3) in mat4 transforms;\n"

          "uniform mat4 u_vp;\n"

          "out vec3 v_normal;\n"
          "out vec3 v_frag_pos;\n"
          "out vec2 v_uv;\n"

          "mat3 extract_rotation_matrix(mat4 modelMatrix) {\n"
          "    return mat3(\n"
          "      modelMatrix[0].xyz,\n"
          "      modelMatrix[1].xyz,\n"
          "      modelMatrix[2].xyz);\n"
          "}\n"

          "void main() {\n"
          "    vec4 model_space = transforms * vec4(a_pos, 1.0);\n"
          "    gl_Position = u_vp * model_space;\n"

          "    v_frag_pos = model_space.xyz;\n"

          "    v_uv = a_uv;\n"

          "    mat3 normal_mat = extract_rotation_matrix(transforms);\n"
          "    v_normal = vec3(normal_mat * a_normals);\n"
          "}\n"),
      STR("#version 430 core\n"

          "out vec4 f_color;\n"

          "in vec3 v_normal;\n"
          "in vec3 v_frag_pos;\n"
          "in vec2 v_uv;\n"

          "uniform vec3 u_light_pos;\n"
          "uniform vec4 u_light_color = vec4(1, 1, 1, 1);\n"
          "uniform vec3 u_view_pos;\n"
          "uniform sampler2D sampler[2];\n"

          "void main() {\n"
          "  vec3 norm = normalize(v_normal);\n"
          // "  f_color = vec4(norm*0.5+0.5, 1); return;\n"

          "  vec3 light_dir = normalize(u_light_pos - v_frag_pos);\n"
          "  vec3 light_color = u_light_color.rgb;\n"

          "  float specular_strenght = 0.5;\n"
          "  vec3 view_dir = normalize(u_view_pos - v_frag_pos);\n"
          "  vec3 reflect_dir = reflect(-light_dir, norm);\n"

          "  const float shininess = texture(sampler[1], v_uv).x;\n"
          "  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), "
          "shininess);\n"
          "  vec3 specular = specular_strenght * spec * light_color;\n"

          "  float ambient_strenght = 0.1;\n"
          "  vec3 ambient = ambient_strenght * light_color;\n"

          "  float diff = max(dot(norm, light_dir), 0.0);\n"
          "  vec3 diffuse = diff * light_color;\n"
          "  vec4 t = texture(sampler[0], v_uv);\n"
          "  vec3 color = (ambient + diffuse + specular) * t.rgb;\n"
          "  f_color = vec4(color, t.a);\n"

          " // // Debugging\n"
          " //  f_color = vec4((v_normal + 1) / 2, 1.0);\n"
          "}\n"),
      ErrPanic);

  test->texture[0] = cm_texture_from_file(
      &scene->gpu, STR("assets/textures/container/texture.png"), ErrPanic);
  test->texture[1] = cm_texture_from_file(
      &scene->gpu, STR("assets/textures/container/specular.png"), ErrPanic);

  RGFW_window_showMouse(window, false);

  glm_vec3_copy((vec3){2, -2, -2}, test->light_pos);
}

static void frame_update(CmScene *scene) {
  Test *test = cm_scene_data(scene);

  cm_camera_update(&test->camera);

  cm_texture_bind(&test->texture[0], 0);
  cm_texture_bind(&test->texture[1], 1);

  cm_shader_bind(&test->shader);
  cm_shader_set_mat4(&test->shader, STR("u_vp"), cm_camera_vp(&test->camera));
  // sandbox->light_pos[0] = sinf(cm_app_time()) * 3;
  // sandbox->light_pos[1] = cosf(cm_app_time()) * 3;
  // sandbox->light_pos[2] = tanf(cm_app_time()) * 3;
  cm_shader_set_vec3(&test->shader, STR("u_light_pos"),
                     test->camera.base.position);
  cm_shader_set_vec3(&test->shader, STR("u_view_pos"),
                     test->camera.base.position);
  cm_shader_set_i32(&test->shader, STR("sampler[0]"), 0);
  cm_shader_set_i32(&test->shader, STR("sampler[1]"), 1);

  cm_mesh_draw_indexed(&test->mesh, CM_DRAW_TRIANGLES);
}

static void final(CmScene *scene) {
  (void)scene;
  RGFW_window *window = cm_app_window();
  RGFW_window_showMouse(window, true);
}

CmSceneInterface *test(void) {
  static CmSceneInterface interface = {
      CM_SCENE(Test), // Scene name
      .init = init,   // init
      .frame_update = frame_update,
      .event = event,
      .final = final,
  };
  return &interface;
}
