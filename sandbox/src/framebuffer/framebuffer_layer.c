#include "claymore.h"

static CmShader layer_shader;
static float zoom;
static float aspect;

static CmShader framebuffer_shader;
static uint32_t fbo;
static uint32_t texture;
static uint32_t rbo;
static CmVertexBuffer buffer;
static CmVertexAttribute attributes;

static bool framebuffer_init(CmScene *scene, CmLayer *layer) {
  (void)scene, (void)layer;
  layer_shader = cm_shader_load_from_file("res/shader/basic.vs.glsl",
                                          "res/shader/basic.fs.glsl");

  zoom = 100.F;
  aspect = (float)scene->app->window->width / (float)scene->app->window->height;
  glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 100.F,
            layer->camera.projection);
  glm_mat4_identity(layer->camera.view);
  glm_vec3_copy((vec3){0}, layer->camera.position);
  glm_translate(layer->camera.view, layer->camera.position);
  layer->camera.update = true;

  /* Frame buffer */
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  // Texture to render the framebuffer to
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scene->app->window->width,
               scene->app->window->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);

  // Specify the attachment if not color attachment.
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture, 0);

  // Render buffer object
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);

  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                        scene->app->window->width, scene->app->window->height);

  // Attach the renderbuffer to framebuffer
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    cm_log_error("Frame buffer was not initialized correctly!\n");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  const float vertecies[] = {
      // positions            // texCoords
      -1.0F, 1.0F,  0.0F, 1.0F, //
      -1.0F, -1.0F, 0.0F, 0.0F, //
      1.0F,  -1.0F, 1.0F, 0.0F, //

      -1.0F, 1.0F,  0.0F, 1.0F, //
      1.0F,  -1.0F, 1.0F, 0.0F, //
      1.0F,  1.0F,  1.0F, 1.0F, //
  };
  const size_t vertices_count = 6;

  buffer = cm_vertex_buffer_create(vertices_count, 4 * sizeof(float), vertecies,
                                   GL_STATIC_DRAW);
  attributes = cm_vertex_attribute_create(&buffer);
  cm_vertex_attribute_push(&attributes, 2, GL_FLOAT, 0);
  cm_vertex_attribute_push(&attributes, 2, GL_FLOAT, sizeof(float) * 2);

  framebuffer_shader = cm_shader_load_from_file(
      "res/shader/framebuffer.vs.glsl", "res/shader/framebuffer.fs.glsl");

  return true;
}

static void framebuffer_free(CmScene *scene, CmLayer *layer) {
  (void)scene, (void)layer;
  // free framebuffer
  glDeleteFramebuffers(1, &fbo);
  glDeleteRenderbuffers(1, &rbo);
  cm_shader_delete(&framebuffer_shader);
  cm_vertex_buffer_delete(&buffer);
  cm_vertex_attribute_delete(&attributes);
}

static void framebuffer_update(CmScene *scene, CmLayer *layer, float dt) {
  (void)scene, (void)layer, (void)dt;

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glClear(GL_COLOR_BUFFER_BIT |
          GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now

  /* RENDER INTO FRAMEBUFFER */
  mat4 model;
  mat4 mvp;

  glm_mat4_identity(model);
  glm_mat4_mul(layer->camera.vp, model, mvp);

  cm_shader_bind(&layer_shader);
  cm_shader_set_mat4(&layer_shader, "u_mvp", mvp);
  cm_renderer2d_begin();
  {
    cm_renderer2d_push_quad_color((vec2){0.F, -100.F}, 0, (vec2){100.F, 100.F},
                                  (vec4){1.F, 0.F, 0.F, 1.F});
  }
  cm_renderer2d_end();
  cm_shader_unbind();
  /* ! RENDER INTO FRAMEBUFFER */

  // Set the framebuffer to the screens frame buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  cm_shader_bind(&framebuffer_shader);
  cm_shader_set_i32(&framebuffer_shader, "u_texture", 0);
  {
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(attributes.id);
    glBindTexture(GL_TEXTURE_2D, texture);

    const uint32_t vertices_count = 6;
    glDrawArrays(GL_TRIANGLES, 0, vertices_count);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
  }
  cm_shader_unbind();
}

CmLayerInterface sandbox_framebuffer(void) {
  return (const CmLayerInterface){
      .init = framebuffer_init,
      .free = framebuffer_free,
      .update = framebuffer_update,
  };
}
