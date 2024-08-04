#include "cases.h"

typedef struct {
  CmCamera2D camera;
  CmFramebuffer fb;
  CmTexture texture;
  CmMesh mesh;
  CmTexture sprite;
  CmShader shader;
} Framebuffer;

static void init(CmScene *scene) {
  Framebuffer *fb = cm_scene_data(scene);
  const float size = 100.f;
  cm_camera2D_ortho(&fb->camera, (vec2){0}, (f32)HEIGHT / (f32)WIDHT, size);

  static u8 data[] = {
      0,   255, 255, //
      255, 0,   255, //
      255, 0,   255, //
      0,   255, 255, //
  };
  fb->sprite = cm_texture_from_bytes(&scene->gpu, (u8 *)data,
                                     (CmTextureFormat){
                                         .w = 2,
                                         .h = 2,
                                         .bpp = 3,
                                         .mag = GL_NEAREST,
                                     });
  fb->fb = cm_framebuffer_create(&scene->gpu, WIDHT, HEIGHT);
  fb->texture = cm_framebuffer_attach_texture_color(&fb->fb);

  vec3 quad[] = {
      {-size, -size, 0}, // top left
      {size, -size, 0},  // top right
      {size, size, 0},   // bottom right

      {-size, -size, 0}, // top left
      {size, size, 0},   // bottom right
      {-size, size, 0},  // bottom left
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

  fb->shader =
      cm_shader_from_memory(&scene->gpu,
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

  cm_2D_quad((vec2){40, -30}, (vec2){40, 40}, (vec4){0.7f, 0.2f, 0.2f, 1});
  cm_2D_quad_rotated((vec2){-30, 25}, (vec2){50, 50}, glm_rad(30), (vec4){0.25, 0.25, 0.8f, 1});
  cm_2D_circle((vec2){-20, -50}, 30, (vec4){0.3f, 0.7f, 0.2f, 1});

  cm_2D_sprite(&fb->sprite, (vec2){31, 45}, (vec2){35, 35});

  cm_2D_line((vec2){20, 30}, (vec2){-30, 50});
  cm_2D_line((vec2){-30, 50}, (vec2){-80, -60});

  cm_2D_end();
  cm_framebuffer_end(&fb->fb);

  cm_texture_bind(&fb->texture, 0);

  cm_shader_bind(&fb->shader);
  cm_shader_set_mat4(&fb->shader, STR("u_mvp"), cm_camera_vp(&fb->camera));

  cm_shader_set_vec2(&fb->shader, STR("light.pos"), (vec2){-0.1f, 0.1f});
  cm_shader_set_f32(&fb->shader, STR("light.radius"), 20.f);
  cm_shader_set_f32(&fb->shader, STR("light.strength"), 100.f);

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
