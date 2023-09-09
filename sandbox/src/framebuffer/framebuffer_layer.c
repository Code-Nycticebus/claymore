#include "claymore.h"
#include "claymore/renderer/renderer2D.h"

static CmShader layer_shader;

static CmShader framebuffer_shader;

static CmFrameBuffer framebuffer;

static CmRenderBuffer rb;

static bool framebuffer_init(CmScene *scene, CmLayer *layer) {
  (void)scene, (void)layer;
  layer_shader = cm_shader_load_from_file("res/shader/basic.vs.glsl",
                                          "res/shader/basic.fs.glsl");

  glm_ortho(0, scene->app->window->width, 0, scene->app->window->height, -1.F,
            100.F, layer->camera.projection);
  glm_mat4_identity(layer->camera.view);
  glm_vec3_copy((vec3){0}, layer->camera.position);
  glm_translate(layer->camera.view, layer->camera.position);
  layer->camera.update = true;

  const float vertecies[] = {
      // positions // texCoords
      -1.0F, 1.0F,  0.0F, 1.0F, //
      -1.0F, -1.0F, 0.0F, 0.0F, //
      1.0F,  -1.0F, 1.0F, 0.0F, //
      1.0F,  1.0F,  1.0F, 1.0F, //
  };
  const size_t vertices_count = 6;

  rb.vertex_buffer = cm_vertex_buffer_create(vertices_count, 4 * sizeof(float),
                                             vertecies, GL_STATIC_DRAW);
  rb.vertex_attribute = cm_vertex_attribute_create(&rb.vertex_buffer);
  cm_vertex_attribute_push(&rb.vertex_attribute, 2, GL_FLOAT, 0);
  cm_vertex_attribute_push(&rb.vertex_attribute, 2, GL_FLOAT,
                           sizeof(float) * 2);

  uint32_t indices[] = {0, 1, 2, 1, 2, 3};
  const size_t indices_count = 6;
  rb.index_buffer = cm_index_buffer_create(&rb.vertex_attribute, indices_count,
                                           indices, GL_STATIC_DRAW);

  framebuffer_shader = cm_shader_load_from_file(
      "res/shader/framebuffer.vs.glsl", "res/shader/framebuffer.fs.glsl");

  framebuffer = cm_framebuffer_create(scene->app->window->width,
                                      scene->app->window->height);

  return true;
}

static void framebuffer_free(CmScene *scene, CmLayer *layer) {
  (void)scene, (void)layer;
  // free framebuffer
  cm_framebuffer_delete(&framebuffer);
  cm_shader_delete(&framebuffer_shader);
  cm_render_buffer_delete(&rb);
}

static void framebuffer_update(CmScene *scene, CmLayer *layer, float dt) {
  (void)scene, (void)layer, (void)dt;

  cm_framebuffer_bind(&framebuffer);
  /* RENDER INTO FRAMEBUFFER */
  mat4 model;
  mat4 mvp;

  glm_mat4_identity(model);
  glm_mat4_mul(layer->camera.vp, model, mvp);

  cm_shader_bind(&layer_shader);
  cm_shader_set_mat4(&layer_shader, "u_mvp", mvp);
  cm_renderer2d_begin();
  {
    cm_renderer2d_push_quad_color((vec2){0.F, 100.F}, 0, (vec2){100.F, 100.F},
                                  (vec4){1.F, 0.F, 0.F, 1.F});
  }
  cm_renderer2d_end();
  cm_shader_unbind();
  /* ! RENDER INTO FRAMEBUFFER */
  cm_framebuffer_unbind();

  cm_framebuffer_draw(&framebuffer, &rb, &framebuffer_shader);
}

CmLayerInterface sandbox_framebuffer(void) {
  return (const CmLayerInterface){
      .init = framebuffer_init,
      .free = framebuffer_free,
      .update = framebuffer_update,
  };
}
