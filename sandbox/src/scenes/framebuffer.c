#include "claymore.h"

typedef struct {
  CmCamera2D camera;
  CmFramebuffer fb;
  CmGpuID texture;
  CmMesh mesh;
  CmShader shader;
} Framebuffer;

static void init(CmScene *scene) {
  Framebuffer *fb = cm_scene_data(scene);
  cm_camera2D_screen(&fb->camera);

  static u8 data[] = {
      255, 1, 1,   255, //
      1,   1, 255, 255, //
      1,   1, 255, 255, //
      255, 1, 1,   255, //
  };
  fb->sprite = cm_texture_from_memory(&scene->gpu, 2, 2, data, CM_TEXTURE_RGBA);

  RGFW_window *win = cm_app_window();
  fb->fb = cm_framebuffer_create(&scene->gpu, win->r.w, win->r.h);
  fb->texture = cm_framebuffer_attach_texture_color(&fb->fb);

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
  fb->mesh = cm_mesh_create(&scene->gpu, ARRAY_LEN(quad), quad);
  cm_mesh_attach_vec2(&fb->mesh, ARRAY_LEN(uv), uv);

  fb->shader = cm_shader_from_memory(
      &scene->gpu,
      STR("#version 430 core\n"
          "layout (location = 0) in vec3 a_pos;\n"
          "layout (location = 1) in vec2 a_uv;\n"
          "uniform mat4 u_mvp;\n"
          "out vec2 v_uv;\n"
          "out vec2 v_pos;\n"
          "void main() {\n"
          "  gl_Position = u_mvp * vec4(a_pos.xyz, 1.0);\n"
          "  v_pos = a_pos.xy;"
          "  v_uv = a_uv;\n"
          "}\n"),
      STR("#version 430 core\n"
          "out vec4 f_color;\n"
          "in vec2 v_uv;\n"
          "in vec2 v_pos;\n"
          "struct LightSource { vec2 pos; float strength; float radius; };\n"
          "uniform LightSource light;\n"
          "float calc_light(vec2 pixel_pos) {\n"
          "   float d = light.strength - distance(light.pos, pixel_pos);\n"
          "   return (d + light.radius) / light.strength;\n"
          "}\n"
          "uniform sampler2D u_sampler;\n"
          "void main() {\n"
          "  f_color = texture(u_sampler, v_uv);\n"
          "  f_color *= calc_light(v_pos);\n"
          "}\n"),
      ErrPanic);
}

static void frame_update(CmScene *scene) {
  Framebuffer *fb = cm_scene_data(scene);

  cm_framebuffer_begin(&fb->fb);
  cm_2D_begin(&fb->camera);

  cm_2D_quad((vec2){10, 30}, (vec2){100, 100}, 0, (vec4){0.7, 0.2, 0.2, 1});
  cm_2D_quad((vec2){310, 90}, (vec2){100, 100}, 0, (vec4){0.25, 0.25, 0.8, 1});
  cm_2D_circle((vec2){140, 260}, 50, (vec4){0.3, 0.7, 0.2, 1});

  cm_2D_sprite(&fb->sprite, (vec2){410, 250}, (vec2){100, 100}, 0, (vec2){0},
               (vec2){1, 1});

  cm_2D_line((vec2){0, 0}, (vec2){100, 50});
  cm_2D_line((vec2){100, 150}, (vec2){200, 330});

  cm_2D_end();
  cm_framebuffer_end();

  cm_shader_bind(&fb->shader);
  cm_shader_set_mat4(&fb->shader, STR("u_mvp"), cm_camera_vp(&fb->camera));

  RGFW_vector m = RGFW_window_getMousePoint(cm_app_window());
  cm_shader_set_vec2(&fb->shader, STR("light.pos"), (vec2){m.x, m.y});
  cm_shader_set_f32(&fb->shader, STR("light.radius"), 20.f);
  cm_shader_set_f32(&fb->shader, STR("light.strength"), 450.f);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fb->texture);

  cm_mesh_draw(&fb->mesh, CM_DRAW_TRIANGLES);
}

CmSceneInterface *framebuffer(void) {
  static CmSceneInterface interface = {
      CM_SCENE(Framebuffer),
      .init = init,
      .frame_update = frame_update,
  };
  return &interface;
}
