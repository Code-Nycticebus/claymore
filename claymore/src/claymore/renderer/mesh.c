#include "mesh.h"
#include <assert.h>

CmMesh cm_mesh_create(vec3s *vertices, size_t count, const uint32_t *indices,
                      size_t indices_count) {
  CmMesh mesh = {0};
  mesh.instance_count = 1;
  mesh.vertices_count = count;

  glGenVertexArrays(1, &mesh.vertex_array);

  glGenBuffers(1, &mesh.vbo.positions);
  glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo.positions);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec3s) * count, vertices,
               GL_STATIC_DRAW);

  glBindVertexArray(mesh.vertex_array);
  glEnableVertexAttribArray(mesh.attrib_index);
  glVertexAttribPointer(mesh.attrib_index, 3, GL_FLOAT, GL_FALSE, sizeof(vec3s),
                        (void *)0);
  mesh.attrib_index++;

  mesh.index_count = indices_count;
  glGenBuffers(1, &mesh.index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices_count,
               indices, GL_STATIC_DRAW);

  return mesh;
}

void cm_mesh_attach_colors(CmMesh *mesh, vec4s *colors, size_t count) {
  assert(!mesh->vbo.color && "Color vector is already initialized!");
  glGenBuffers(1, &mesh->vbo.color);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo.color);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec4s) * count, colors, GL_STATIC_DRAW);

  glBindVertexArray(mesh->vertex_array);
  glEnableVertexAttribArray(mesh->attrib_index);
  glVertexAttribPointer(mesh->attrib_index, 4, GL_FLOAT, GL_FALSE,
                        sizeof(vec4s), (void *)0);
  mesh->attrib_index++;
}
void cm_mesh_attach_colors_instanced(CmMesh *mesh, vec4s *colors,
                                     size_t count) {
  assert(!mesh->vbo.color && "Color vector is already initialized!");
  glGenBuffers(1, &mesh->vbo.color);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo.color);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec4s) * count, colors, GL_STATIC_DRAW);

  glBindVertexArray(mesh->vertex_array);
  glEnableVertexAttribArray(mesh->attrib_index);
  glVertexAttribPointer(mesh->attrib_index, 4, GL_FLOAT, GL_FALSE,
                        sizeof(vec4s), (void *)0);
  glVertexAttribDivisor(mesh->attrib_index, 1);
  mesh->attrib_index++;
}

void cm_mesh_update_colors(CmMesh *mesh, vec4s *colors, size_t count) {
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo.color);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4s) * count, colors);
  mesh->instance_count = count;
}

void cm_mesh_attach_normals(CmMesh *mesh, vec3s *normals, size_t count) {
  assert(!mesh->vbo.normal && "Normal vector is already initialized!");
  glGenBuffers(1, &mesh->vbo.normal);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo.normal);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec3s) * count, normals, GL_STATIC_DRAW);

  glBindVertexArray(mesh->vertex_array);
  glEnableVertexAttribArray(mesh->attrib_index);
  glVertexAttribPointer(mesh->attrib_index, 3, GL_FLOAT, GL_FALSE,
                        sizeof(vec3s), (void *)0);

  mesh->attrib_index++;
}

void cm_mesh_attach_uv(CmMesh *mesh, vec2s *uv, size_t count) {
  assert(!mesh->vbo.uv && "UV vector is already initialized!");
  glGenBuffers(1, &mesh->vbo.uv);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo.uv);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec2s) * count, uv, GL_STATIC_DRAW);

  glBindVertexArray(mesh->vertex_array);
  glEnableVertexAttribArray(mesh->attrib_index);
  glVertexAttribPointer(mesh->attrib_index, 2, GL_FLOAT, GL_FALSE,
                        sizeof(vec4s), (void *)0);

  mesh->attrib_index++;
}

void cm_mesh_attach_transforms(CmMesh *mesh, mat4s *transforms, size_t count) {
  assert(!mesh->vbo.transforms && "Transform vector is already initialized!");
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
