#include "claymore.h"
#include <string.h>

static CmShader layer_shader;
static CmShader texture_shader;

static CmShader framebuffer_shader;
static CmShader effect_shader;

const char *vs_file = "res/shader/framebuffer.vs.glsl";
char *fs_file = NULL;

static CmFrameBuffer framebuffer;
static CmFrameBuffer framebuffer2;

static CmTexture2D texture;
static CmRenderBuffer rb;
static CmRenderBuffer rb2;

static bool effect = true;

static char *_strdup(const char *str) {
  size_t size = strlen(str) + 1;
  char *s = calloc(size, sizeof(char));
  strcpy(s, str);
  return s;
}

static void reload_shader(void) {
  // Reload effect shader
  cm_shader_delete(&effect_shader);
  effect_shader = cm_shader_load_from_file(vs_file, fs_file);
}

static void key_callback(CmKeyEvent *event, void *data) {
  (void)data;
  if (event->action == CM_KEY_PRESS) {
    if (event->code == CM_KEY_F4) {
      effect = !effect;
    } else if (event->code == CM_KEY_F5) {
      reload_shader();
    }
  }
}

void drop_callback(GLFWwindow *window, int count, const char **paths) {
  (void)window;
  for (int i = 0; i < count; i++) {
    free(fs_file);
    fs_file = _strdup(paths[i]);
    reload_shader();
  }
}

static bool framebuffer_init(CmScene *scene, CmLayer *layer) {
  (void)scene, (void)layer;
  layer_shader = cm_shader_load_from_file("res/shader/basic.vs.glsl",
                                          "res/shader/basic.fs.glsl");

  layer->camera.projection = glms_ortho(
      0, scene->app->window->width, 0, scene->app->window->height, -1.F, 100.F);
  layer->camera.view = glms_mat4_identity();
  layer->camera.position = (vec3s){0};
  layer->camera.view =
      glms_translate(layer->camera.view, layer->camera.position);
  layer->camera.update = true;

  const float vertecies[] = {
      // positions // texCoords
      -1.0F, -1.0F, 0.0F, 0.0F, //
      1.0F,  -1.0F, 1.0F, 0.0F, //
      1.0F,  1.0F,  1.0F, 1.0F, //
      -1.0F, 1.0F,  0.0F, 1.0F, //
  };
  const size_t vertices_count = 4;

  rb.vertex_buffer = cm_vertex_buffer_create(vertices_count, 4 * sizeof(float),
                                             vertecies, GL_STATIC_DRAW);
  rb.vertex_attribute = cm_vertex_attribute_create(&rb.vertex_buffer);
  cm_vertex_attribute_push(&rb.vertex_attribute, 2, GL_FLOAT, 0);
  cm_vertex_attribute_push(&rb.vertex_attribute, 2, GL_FLOAT,
                           sizeof(float) * 2);

  uint32_t indices[] = {0, 1, 3, 1, 2, 3};
  const size_t indices_count = 6;
  rb.index_buffer = cm_index_buffer_create(&rb.vertex_attribute, indices_count,
                                           indices, GL_STATIC_DRAW);

  framebuffer_shader = cm_shader_load_from_file(
      "res/shader/framebuffer.vs.glsl", "res/shader/framebuffer.fs.glsl");

  fs_file = _strdup("res/shader/effect.fs.glsl");
  effect_shader = cm_shader_load_from_file(vs_file, fs_file);

  framebuffer = cm_framebuffer_create(scene->app->window->width,
                                      scene->app->window->height);

  const float vertecies2[] = {
      // positions // texCoords
      -1.0F, -1.0F, 0.0F, 0.0F, //
      -.5F,  -1.0F, 1.0F, 0.0F, //
      -.5F,  -.5F,  1.0F, 1.0F, //
      -1.0F, -.5F,  0.0F, 1.0F, //
  };
  const size_t vertices_count2 = 4;

  rb2.vertex_buffer = cm_vertex_buffer_create(
      vertices_count2, 4 * sizeof(float), vertecies2, GL_STATIC_DRAW);
  rb2.vertex_attribute = cm_vertex_attribute_create(&rb2.vertex_buffer);
  cm_vertex_attribute_push(&rb2.vertex_attribute, 2, GL_FLOAT, 0);
  cm_vertex_attribute_push(&rb2.vertex_attribute, 2, GL_FLOAT,
                           sizeof(float) * 2);

  rb2.index_buffer = cm_index_buffer_create(
      &rb2.vertex_attribute, indices_count, indices, GL_STATIC_DRAW);

  framebuffer2 = cm_framebuffer_create(scene->app->window->width,
                                       scene->app->window->height);

  texture_shader = cm_shader_load_from_file("res/shader/texture.vs.glsl",
                                            "res/shader/texture.fs.glsl");
  texture = cm_texture2d_create("res/textures/claymore-sword.png");

  cm_event_set_callback(CM_EVENT_KEYBOARD, (cm_event_callback)key_callback,
                        NULL);

  glfwSetDropCallback(scene->app->window->ctx, drop_callback);

  return true;
}

static void framebuffer_free(CmScene *scene, CmLayer *layer) {
  (void)scene, (void)layer;
  free(fs_file);
  // free framebuffer
  cm_framebuffer_delete(&framebuffer);
  cm_framebuffer_delete(&framebuffer2);
  cm_shader_delete(&framebuffer_shader);
  cm_shader_delete(&effect_shader);
  cm_render_buffer_delete(&rb);
  cm_render_buffer_delete(&rb2);
}

static void framebuffer_update(CmScene *scene, CmLayer *layer, float dt) {
  (void)scene, (void)layer, (void)dt;

  cm_framebuffer_bind(&framebuffer);
  /* RENDER INTO FRAMEBUFFER */
  mat4s model;
  mat4s mvp;

  model = glms_mat4_identity();
  mvp = glms_mat4_mul(layer->camera.vp, model);

  cm_shader_bind(&layer_shader);
  cm_shader_set_mat4(&layer_shader, "u_mvp", mvp);
  cm_renderer2d_begin();
  {
    cm_renderer2d_push_quad_color((vec2s){{125.F, 100.F}}, 0,
                                  (vec2s){{100.F, 100.F}},
                                  (vec4s){{1.F, 0.F, 0.F, 1.F}});
  }
  cm_renderer2d_end();
  cm_shader_unbind();

  cm_texture_bind(&texture, 0);
  cm_shader_bind(&texture_shader);
  cm_shader_set_mat4(&texture_shader, "u_mvp", mvp);
  cm_shader_set_i32(&texture_shader, "u_texture", 0);
  cm_renderer2d_begin();
  {
    cm_renderer2d_push_quad_textured((vec2s){{285.F, 120.F}}, 0,
                                     (vec2s){{250.F, 250.F}}, (vec2s){{0, 0}},
                                     (vec2s){{1, 1}});
  }
  cm_renderer2d_end();
  cm_shader_unbind();
  /* ! RENDER INTO FRAMEBUFFER */

  cm_framebuffer_bind(&framebuffer2);
  cm_framebuffer_draw(&framebuffer, &rb, &framebuffer_shader);

  cm_framebuffer_unbind();
  cm_framebuffer_draw(&framebuffer, &rb,
                      effect ? &effect_shader : &framebuffer_shader);
  cm_framebuffer_draw(&framebuffer2, &rb2, &framebuffer_shader);
}

CmLayerInterface sandbox_framebuffer(void) {
  return (const CmLayerInterface){
      .init = framebuffer_init,
      .free = framebuffer_free,
      .update = framebuffer_update,
  };
}
