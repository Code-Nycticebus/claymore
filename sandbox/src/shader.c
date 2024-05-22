#include "claymore/app/scene.h"
#include "claymore/entrypoint.h"

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

static CmScene *shader_init(CmScene *parent, Str filename) {
  CmScene *scene = cm_scene_push(parent, shader);
  ShaderToy *toy = scene->data;

  cm_window_get_size(toy->resolution);

  Arena arena = {0};
  Str content = file_read_str(filename, &arena, ErrPanic);
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
                            content, ErrPanic);
  arena_free(&arena);
  return scene;
}

typedef struct {
  Error error;
  CmScene *child;
  Str filename;
} Toy;

static void toy_init(CmScene *scene) {
  Toy *toy = cm_scene_alloc_data(scene, sizeof(Toy));

  toy->filename = STR("assets/shader/toy.fs.glsl");
  toy->child = shader_init(scene, toy->filename);
}

static void event(CmScene *scene, CmEvent *event) {
  Toy *toy = scene->data;
  cm_event_key(event, {
    if (key->action == CM_KEY_RELEASE) {
      if (key->code == CM_KEY_ESCAPE) {
        cm_window_close();
        return;
      }
      if (key->code == CM_KEY_F5) {
        // Releoad
        cm_scene_delete(scene, toy->child);
        toy->child = shader_init(scene, toy->filename);
      }
    }
  });
}

static CmSceneInterface *toy(void) {
  static CmSceneInterface interface = {
      .init = toy_init,
      .event = event,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 720, .height = 420, .title = "Shader Toy"},
      .main = toy,
  };
  return &config;
}
