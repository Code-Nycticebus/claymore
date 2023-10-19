#include "mesh.h"
#include <assert.h>

CmMesh cm_mesh_create(const uint32_t *indices, size_t indices_count) {
  CmMesh mesh;
  mesh.instance_count = 1;
  glGenVertexArrays(1, &mesh.vertex_array);
  glBindVertexArray(mesh.vertex_array);

  mesh.index_count = indices_count;
  glGenBuffers(1, &mesh.index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices_count,
               indices, GL_STATIC_DRAW);

  return mesh;
}

void cm_mesh_push_positions(CmMesh *mesh, vec3s *vertices, size_t count) {

  glGenBuffers(1, &mesh->vbo.positions);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo.positions);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec3s) * count, vertices,
               GL_STATIC_DRAW);

  glBindVertexArray(mesh->vertex_array);
  glEnableVertexAttribArray(mesh->attrib_index);
  glVertexAttribPointer(mesh->attrib_index, 3, GL_FLOAT, GL_FALSE,
                        sizeof(vec3s), (void *)0);

  mesh->attrib_index++;
}
void cm_mesh_push_colors(CmMesh *mesh, vec4s *colors, size_t count) {
  glGenBuffers(1, &mesh->vbo.color);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo.color);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec4s) * count, colors, GL_STATIC_DRAW);

  glBindVertexArray(mesh->vertex_array);
  glEnableVertexAttribArray(mesh->attrib_index);
  glVertexAttribPointer(mesh->attrib_index, 4, GL_FLOAT, GL_FALSE,
                        sizeof(vec4s), (void *)0);

  mesh->attrib_index++;
}

void cm_mesh_push_transforms(CmMesh *mesh, mat4s *transforms, size_t count) {
  glGenBuffers(1, &mesh->vbo.transforms);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo.transforms);
  glBufferData(GL_ARRAY_BUFFER, sizeof(mat4s) * count, transforms,
               GL_DYNAMIC_DRAW);

  mesh->instance_count = count;

  glBindVertexArray(mesh->vertex_array);
  for (size_t i = 0; i < 4; i++) {
    glEnableVertexAttribArray(mesh->attrib_index);
    glVertexAttribPointer(mesh->attrib_index, 4, GL_FLOAT, GL_FALSE,
                          sizeof(mat4s), (void *)(sizeof(vec4s) * i)); // NOLINT
    glVertexAttribDivisor(mesh->attrib_index, 1);
    mesh->attrib_index++;
  }
}

void cm_mesh_update_transforms(CmMesh *mesh, mat4s *transforms, size_t count) {
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo.transforms);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(mat4s) * count, transforms);
  mesh->instance_count = count;
}

void cm_mesh_draw(CmMesh *mesh) {
  glBindVertexArray(mesh->vertex_array);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
  glDrawElementsInstanced(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT,
                          NULL, mesh->instance_count);
}
