#include "renderer_lines.h"

#include "claymore/renderer/render_buffer.h"

#include "renderer_defines.h"

#define CM_RENDERER_LINES_MAX 1000

typedef struct {
  vec3s pos;
  vec4s color;
} CmLineVertex;

typedef struct {
  CmVertexBuffer vbo;
  CmVertexArray vao;
  size_t vertices_count;
  CmLineVertex data[CM_RENDERER_LINES_MAX];
} CmLineRendererData;

static CmLineRendererData *render_data;

void cm_renderer_lines_init(void) {
  render_data = malloc(sizeof(CmLineRendererData));
  render_data->vbo = cm_vertex_buffer_create(
      NULL, sizeof(CmLineVertex) * CM_RENDERER_LINES_MAX, CM_BUFFER_DYNAMIC);

  render_data->vao = cm_vertex_array_create();
  cm_vertex_array_push_attrib(&render_data->vao, 3, sizeof(CmLineVertex),
                              offsetof(CmLineVertex, pos));
  cm_vertex_array_push_attrib(&render_data->vao, 4, sizeof(CmLineVertex),
                              offsetof(CmLineVertex, color));

  render_data->vertices_count = 0;
}

static void cm_renderer_lines_flush(void) {
  glBindBuffer(GL_ARRAY_BUFFER, render_data->vbo.id);
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  sizeof(CmLineVertex) * render_data->vertices_count,
                  render_data->data);
  glBindVertexArray(render_data->vao.id);
  glDrawArrays(GL_LINE_STRIP, 0, render_data->vertices_count);
  render_data->vertices_count = 0;

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void cm_renderer_lines_begin(void) {}
void cm_renderer_lines_end(void) { cm_renderer_lines_flush(); }

void cm_renderer_lines_push(vec3s pos, vec4s color) {
  if (!(render_data->vertices_count < CM_RENDERER_LINES_MAX)) {
    cm_renderer_lines_flush();
  }
  render_data->data[render_data->vertices_count].pos = pos;
  render_data->data[render_data->vertices_count].color = color;
  render_data->vertices_count++;
}

void cm_renderer_lines_shutdown(void) {
  cm_vertex_buffer_delete(&render_data->vbo);
  free(render_data);
}
