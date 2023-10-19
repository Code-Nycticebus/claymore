#include "mesh.h"
#include <stdint.h>

struct Transforms {
  CmVertexBuffer vbo;
  size_t cap;
  size_t count;
  mat4s matrices[];
};

static Transforms *transforms_create(CmVertexAttribute *vertex_attribute,
                                     size_t size) {
  Transforms *transforms =
      malloc(sizeof(Transforms) + sizeof(transforms->matrices[0]) * size);
  assert(transforms);
  transforms->cap = size;
  transforms->count = 0;

  transforms->vbo = cm_vertex_buffer_create(
      1, sizeof(transforms->matrices[0]) * size, NULL, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, transforms->vbo.id);
  glBindVertexArray(vertex_attribute->id);

  for (size_t i = 0; i < 4; i++) {
    glEnableVertexAttribArray(vertex_attribute->index);
    glVertexAttribPointer(vertex_attribute->index, 4, GL_FLOAT, GL_TRUE,
                          sizeof(transforms->matrices[0]),
                          (void *)(sizeof(vec4s) * i)); // NOLINT
    glVertexAttribDivisor(vertex_attribute->index, 1);
    vertex_attribute->index++;
  }
  glBindVertexArray(0);

  return transforms;
}

static void transforms_delete(Transforms *transforms) { free(transforms); }

CmMesh *cm_mesh_create(CmRenderBuffer *buffer, bool static_mesh,
                       size_t transforms_count) {
  CmMesh *mesh = malloc(sizeof(CmMesh));
  assert(mesh);
  mesh->buffer = buffer;
  mesh->static_mesh = static_mesh;

  mesh->transforms = NULL;
  if (transforms_count) {
    mesh->transforms =
        transforms_create(&mesh->buffer->vertex_attribute, transforms_count);
  }
  return mesh;
}

void cm_mesh_delete(CmMesh *mesh) {
  transforms_delete(mesh->transforms);
  free(mesh);
}

void cm_mesh_transform_push(CmMesh *mesh, mat4s transform) {
  assert(mesh->transforms->count < mesh->transforms->cap);
  mesh->transforms->matrices[mesh->transforms->count] = transform;
  mesh->transforms->count++;
}

void cm_mesh_transform_clear(CmMesh *mesh) { mesh->transforms->count = 0; }

void cm_mesh_draw(CmMesh *mesh) {
  glBindBuffer(GL_ARRAY_BUFFER, mesh->transforms->vbo.id);
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  sizeof(mesh->transforms->matrices[0]) *
                      mesh->transforms->count,
                  mesh->transforms->matrices);
  glBindVertexArray(mesh->buffer->vertex_attribute.id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->buffer->index_buffer.id);
  glDrawElementsInstanced(GL_TRIANGLES, mesh->buffer->index_buffer.count,
                          GL_UNSIGNED_INT, NULL, mesh->transforms->count);
}
