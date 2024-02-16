#include "mesh.h"

#include "GL/glew.h"

CmMesh cm_mesh_create(Arena *arena, const vec3 *vertices, usize count) {
  CmMesh mesh = {0};
  mesh.instance_count = 1;

  mesh.vertices_count = count;
  da_init(&mesh.vbo, arena);

  glGenVertexArrays(1, &mesh.vao);

  glGenBuffers(1, &mesh.vertices);
  glBindBuffer(GL_ARRAY_BUFFER, mesh.vertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * count, vertices, GL_STATIC_DRAW);

  glBindVertexArray(mesh.vao);
  glEnableVertexAttribArray(mesh.idx);
  glVertexAttribPointer(mesh.idx, 3, GL_FLOAT, GL_FALSE, sizeof(vec3),
                        (void *)0);
  mesh.idx++;

  return mesh;
}

void cm_mesh_delete(CmMesh *mesh) {
  glDeleteVertexArrays(1, &mesh->vao);
  glDeleteBuffers(1, &mesh->ebo);
  glDeleteBuffers(mesh->vbo.len, mesh->vbo.items);
}

void cm_mesh_attach_index_buffer(CmMesh *mesh, const u32 *indices,
                                 usize count) {
  glBindVertexArray(mesh->vao);
  mesh->index_count = count;
  glGenBuffers(1, &mesh->ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * count, indices,
               GL_STATIC_DRAW);
}

void cm_mesh_attach_vec3(CmMesh *mesh, const vec3 *v, usize count) {
  u32 vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * count, v, GL_STATIC_DRAW);
  da_push(&mesh->vbo, vbo);

  glBindVertexArray(mesh->vao);
  glEnableVertexAttribArray(mesh->idx);
  glVertexAttribPointer(mesh->idx, 3, GL_FLOAT, GL_FALSE, sizeof(vec3),
                        (void *)0);
  mesh->idx++;
}

void cm_mesh_attach_vec3_instanced(CmMesh *mesh, const vec3 *v, usize count) {
  u32 vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * count, v, GL_STATIC_DRAW);
  da_push(&mesh->vbo, vbo);

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

// void cm_mesh_update_colors(CmMesh *mesh, vec4s *colors, usize count) {
//   glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo.color);
//   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4s) * count, colors);
//   mesh->instance_count = count;
// }

// void cm_mesh_attach_normals(CmMesh *mesh, vec3s *normals, usize count) {
//   assert(!mesh->vbo.normal && "Normal vector is already initialized!");
//   glGenBuffers(1, &mesh->vbo.normal);
//   glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo.normal);
//   glBufferData(GL_ARRAY_BUFFER, sizeof(vec3s) * count, normals,
//   GL_STATIC_DRAW);

//   glBindVertexArray(mesh->vertex_array);
//   glEnableVertexAttribArray(mesh->attrib_index);
//   glVertexAttribPointer(mesh->attrib_index, 3, GL_FLOAT, GL_FALSE,
//                         sizeof(vec3s), (void *)0);

//   mesh->attrib_index++;
// }

// void cm_mesh_attach_uv(CmMesh *mesh, vec2s *uv, usize count) {
//   assert(!mesh->vbo.uv && "UV vector is already initialized!");
//   glGenBuffers(1, &mesh->vbo.uv);
//   glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo.uv);
//   glBufferData(GL_ARRAY_BUFFER, sizeof(vec2s) * count, uv, GL_STATIC_DRAW);

//   glBindVertexArray(mesh->vertex_array);
//   glEnableVertexAttribArray(mesh->attrib_index);
//   glVertexAttribPointer(mesh->attrib_index, 2, GL_FLOAT, GL_FALSE,
//                         sizeof(vec2s), (void *)0);

//   mesh->attrib_index++;
// }

// void cm_mesh_attach_transforms(CmMesh *mesh, mat4s *transforms, usize count)
// {
//   assert(!mesh->vbo.transforms && "Transform vector is already
//   initialized!"); glGenBuffers(1, &mesh->vbo.transforms);
//   glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo.transforms);
//   glBufferData(GL_ARRAY_BUFFER, sizeof(mat4s) * count, transforms,
//                GL_DYNAMIC_DRAW);
//   mesh->instance_count = count;

//   glBindVertexArray(mesh->vertex_array);
//   for (usize i = 0; i < 4; i++) {
//     glEnableVertexAttribArray(mesh->attrib_index);
//     glVertexAttribPointer(mesh->attrib_index, 4, GL_FLOAT, GL_FALSE,
//                           sizeof(mat4s), (void *)(sizeof(vec4s) * i)); //
//                           NOLINT
//     glVertexAttribDivisor(mesh->attrib_index, 1);
//     mesh->attrib_index++;
//   }
// }

// void cm_mesh_update_transforms(CmMesh *mesh, mat4s *transforms, usize count)
// {
//   glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo.transforms);
//   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(mat4s) * count, transforms);
//   mesh->instance_count = count;
// }

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
