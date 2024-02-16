#include "mesh.h"

#include "GL/glew.h"
#include "claymore/renderer/buffer.h"

CmMesh cm_mesh_create(CmBuffers *b, usize count, const vec3 *vertices) {
  CmMesh mesh = {0};
  mesh.instance_count = 1;
  mesh.buffer = b;

  mesh.vertices_count = count;

  cm_buffer_vbo(b, CM_STATIC_DRAW, sizeof(vec3) * count, &vertices[0][0]);

  mesh.vao = cm_buffer_vao(b);

  glEnableVertexAttribArray(mesh.idx);
  glVertexAttribPointer(mesh.idx, 3, GL_FLOAT, GL_FALSE, sizeof(vec3),
                        (void *)0);
  mesh.idx++;

  return mesh;
}

void cm_mesh_attach_index_buffer(CmMesh *mesh, usize count,
                                 const u32 *indices) {
  glBindVertexArray(mesh->vao);
  mesh->index_count = count;
  mesh->ebo = cm_buffer_ebo(mesh->buffer, CM_STATIC_DRAW, count, indices);
}

void cm_mesh_attach_vec3(CmMesh *mesh, usize count, const vec3 *v) {
  cm_buffer_vbo(mesh->buffer, CM_STATIC_DRAW, sizeof(vec3) * count, &v[0][0]);

  glBindVertexArray(mesh->vao);
  glEnableVertexAttribArray(mesh->idx);
  glVertexAttribPointer(mesh->idx, 3, GL_FLOAT, GL_FALSE, sizeof(vec3),
                        (void *)0);
  mesh->idx++;
}

void cm_mesh_attach_vec3_instanced(CmMesh *mesh, usize count, const vec3 *v) {
  cm_buffer_vbo(mesh->buffer, CM_STATIC_DRAW, sizeof(vec3) * count, &v[0][0]);

  glBindVertexArray(mesh->vao);
  glEnableVertexAttribArray(mesh->idx);
  glVertexAttribPointer(mesh->idx, 3, GL_FLOAT, GL_FALSE, sizeof(vec3),
                        (void *)0);
  glVertexAttribDivisor(mesh->idx, 1);
  mesh->idx++;

  clib_assert(mesh->instance_count == 1 || mesh->instance_count == count,
              "This would result in a crash");
  mesh->instance_count = count;
}

void cm_mesh_attach_vec4(CmMesh *mesh, usize count, const vec4 *v) {
  cm_buffer_vbo(mesh->buffer, CM_STATIC_DRAW, sizeof(vec4) * count, &v[0][0]);

  glBindVertexArray(mesh->vao);
  glEnableVertexAttribArray(mesh->idx);
  glVertexAttribPointer(mesh->idx, 4, GL_FLOAT, GL_FALSE, sizeof(vec4),
                        (void *)0);
  mesh->idx++;
}

void cm_mesh_attach_vec4_instanced(CmMesh *mesh, usize count, const vec4 *v) {
  cm_buffer_vbo(mesh->buffer, CM_STATIC_DRAW, sizeof(vec4) * count, &v[0][0]);

  glBindVertexArray(mesh->vao);
  glEnableVertexAttribArray(mesh->idx);
  glVertexAttribPointer(mesh->idx, 4, GL_FLOAT, GL_FALSE, sizeof(vec4),
                        (void *)0);
  glVertexAttribDivisor(mesh->idx, 1);
  mesh->idx++;

  clib_assert(mesh->instance_count == 1 || mesh->instance_count == count,
              "This would result in a crash");
  mesh->instance_count = count;
}

void cm_mesh_draw_indexed(CmMesh *mesh) {
  glBindVertexArray(mesh->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
  glDrawElementsInstanced(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT,
                          NULL, mesh->instance_count);
}

void cm_mesh_draw(CmMesh *mesh) {
  glBindVertexArray(mesh->vao);
  glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->vertices_count,
                        mesh->instance_count);
}
