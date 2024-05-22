#include "cebus/core/error.h"
#include "shader_toy.h"

#include "glad.h"

typedef struct {
  CmShader shader;
  float time;
  vec2 resolution;
  i32 frame;
} ShaderToy;

static void init(CmScene *scene) {
  (void)cm_scene_alloc_data(scene, sizeof(ShaderToy));
}

static void update(CmScene *scene, double dt) {
  ShaderToy *toy = scene->data;
  toy->time += dt;
  toy->frame++;

  cm_shader_bind(&toy->shader);
  cm_shader_set_f32(&toy->shader, STR("u_time"), toy->time);
  cm_shader_set_i32(&toy->shader, STR("u_frame"), toy->frame);
  cm_shader_set_vec2(&toy->shader, STR("u_resolution"), toy->resolution);

  cm_shader_set_f32(&toy->shader, STR("u_deltatime"), dt);
  cm_shader_set_f32(&toy->shader, STR("u_fps"), 1 / dt);

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);
}

static CmSceneInterface *shader(void) {
  static CmSceneInterface interface = {
      .init = init,
      .update = update,
  };
  return &interface;
}

CmScene *shader_init(CmScene *parent, Str filename, Error *error) {
  CmScene *scene = cm_scene_push(parent, shader);
  ShaderToy *toy = scene->data;

  cm_window_get_size(toy->resolution);

  Arena arena = {0};
  Str content = file_read_str(filename, &arena, ErrPanic);

  StringBuilder sb = sb_init(&arena);
  sb_append_str(&sb, STR("#version 430 core\n"
                         "layout(location = 0) out vec4 f_color;\n"
                         "uniform float u_time;\n"
                         "uniform int u_frame;\n"
                         "uniform vec2 u_resolution;\n"
                         "uniform float u_deltatime;\n"
                         "uniform float u_fps;\n"));
  sb_append_str(&sb, content);
  sb_append_str(&sb, STR("void main() {\n"
                         "  mainImage(f_color, gl_FragCoord.xy);\n"
                         "}\n"));

  toy->shader =
      cm_shader_from_memory(&scene->gpu,
                            STR("#version 430 core\n"
                                "void main() {\n"
                                "  gl_Position = vec4(\n"
                                "    (gl_VertexID >> 0 & 1) * 2 - 1,\n"
                                "    (gl_VertexID >> 1 & 1) * 2 - 1,\n"
                                "    0.0,\n"
                                "    1.0\n"
                                "  );\n"
                                "}\n"),
                            sb_to_str(&sb), error);
  arena_free(&arena);

  return scene;
}
