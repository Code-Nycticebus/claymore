#include "claymore.h"

typedef struct {
  CmCamera2D camera;
  CmFramebuffer fb;
  CmMesh mesh;
  CmShader shader;
} Test;

static void init(CmScene *scene) {
  Test *test = cm_scene_data(scene);
  cm_camera2D_screen(&test->camera);

  RGFW_window *win = cm_app_window();
  test->fb = cm_framebuffer_create(&scene->gpu, win->r.w, win->r.h);
  cm_framebuffer_attach_texture_color(&test->fb);

  const float w = win->r.w;
  const float h = win->r.h;

  vec3 quad[] = {
      {0, 0, 0}, // top left
      {w, 0, 0}, // top right
      {w, h, 0}, // bottom right

      {0, 0, 0}, // top left
      {w, h, 0}, // bottom right
      {0, h, 0}, // bottom left
  };
  vec2 uv[] = {
      {0, 1}, // tex top left
      {1, 1}, // tex top right
      {1, 0}, // tex bottom right

      {0, 1}, // tex top left
      {1, 0}, // tex bottom right
      {0, 0}, // tex bottom left
  };
  test->mesh = cm_mesh_create(&scene->gpu, ARRAY_LEN(quad), quad);
  cm_mesh_attach_vec2(&test->mesh, ARRAY_LEN(uv), uv);

  test->shader = cm_shader_from_memory(
      &scene->gpu,
      STR("#version 430 core\n"
          "layout (location = 0) in vec3 a_pos;\n"
          "layout (location = 1) in vec2 a_uv;\n"
          "uniform mat4 u_mvp;\n"
          "out vec2 v_uv;\n"
          "void main() {\n"
          "  gl_Position = u_mvp * vec4(a_pos.xyz, 1.0);\n"
          "  v_uv = a_uv;\n"
          "}\n"),
      STR("#version 430 core\n"
          "out vec4 f_color;\n"
          "in vec2 v_uv;\n"
          "uniform sampler2D u_sampler;\n"
          "void main() {\n"
          "  f_color = texture(u_sampler, v_uv);\n"
          "  f_color.a *= 0.25;\n"
          "}\n"),
      ErrPanic);
}

static void frame_update(CmScene *scene) {
  Test *test = cm_scene_data(scene);
  (void)test;

  cm_framebuffer_begin(&test->fb);
  cm_2D_begin(&test->camera);
  cm_2D_quad((vec2){10, 30}, (vec2){100, 100}, 0, (vec4){0.7, 0.2, 0.2, 1});
  cm_2D_quad((vec2){310, 90}, (vec2){100, 100}, 0, (vec4){0.25, 0.25, 0.8, 1});
  cm_2D_quad((vec2){120, 160}, (vec2){100, 100}, 0, (vec4){0.3, 0.7, 0.2, 1});

  cm_2D_line((vec2){0, 0}, (vec2){100, 50});
  cm_2D_line((vec2){100, 150}, (vec2){200, 330});

  cm_2D_end();
  cm_framebuffer_end();

  cm_shader_bind(&test->shader);
  cm_shader_set_mat4(&test->shader, STR("u_mvp"), cm_camera_vp(&test->camera));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, test->fb.color);

  cm_mesh_draw(&test->mesh, CM_DRAW_TRIANGLES);
}

CmSceneInterface *test(void) {
  static CmSceneInterface interface = {
      CM_SCENE(Test),
      .init = init,
      .frame_update = frame_update,
  };
  return &interface;
}
