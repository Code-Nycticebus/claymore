#include "claymore/app/entrypoint.h"

#include "claymore/app/window.h"
#include "claymore/renderer/shaders.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

typedef struct {
  u32 VAO, VBO, EBO;
  CmShader shader;
} Sandbox;

static void sandbox_init(CmScene *scene) {
  clib_log_info("sandbox init");
  Sandbox *sandbox = arena_alloc(&scene->arena, sizeof(Sandbox));

  vec3 vertices[4] = {
      {-.5f, 0.5f, .0f},
      {-.5f, -.5f, .0f},
      {0.5f, -.5f, .0f},
      {0.5f, 0.5f, .0f},
  };
  u32 indices[6] = {
      0, 1, 3, 1, 2, 3,
  };

  glGenBuffers(1, &sandbox->VBO);
  glBindBuffer(GL_ARRAY_BUFFER, sandbox->VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &sandbox->VAO);
  glBindVertexArray(sandbox->VAO);

  usize idx = 0;
  glVertexAttribPointer(idx, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), NULL);
  glEnableVertexAttribArray(idx);
  idx++;

  glGenBuffers(1, &sandbox->EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sandbox->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  sandbox->shader = cm_shader_load_from_memory(
      STR("#version 330 core\n"
          "layout (location = 0) in vec3 a_pos;\n"
          "void main() {\n"
          "  gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);\n"
          "}\n"),
      STR("#version 330 core\n"
          "out vec4 f_color;\n"
          "void main() {\n"
          "  f_color = vec4(1.0, 0.0, 0.0, 1.0);\n"
          "}\n"),
      ErrPanic);

  scene->data = sandbox;
}

static void sandbox_update(CmScene *scene) {
  Sandbox *sandbox = scene->data;

  glUseProgram(sandbox->shader.id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sandbox->EBO);
  glBindVertexArray(sandbox->VAO);
  glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1);

  if (glfwGetKey(cm_window_context(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(cm_window_context(), true);
  }
}

static void sandbox_free(CmScene *scene) {
  (void)scene;
  clib_log_info("sandbox free");
}

static CmSceneInterface *sandbox_scene_init(void) {
  static CmSceneInterface sandbox = {
      .init = sandbox_init,
      .update = sandbox_update,
      .free = sandbox_free,
  };
  return &sandbox;
}

ClaymoreConfig claymore_init(void) {
  return (ClaymoreConfig){
      .window = {.width = 720, .height = 420, .title = "sandbox"}, // NOLINT
      .scene = sandbox_scene_init,
  };
}
