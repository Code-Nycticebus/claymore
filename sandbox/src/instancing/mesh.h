#pragma once

#include <GL/glew.h>
#include <cglm/struct.h>

#define MESH_MAX_VBO 5

typedef struct {
  struct {
    GLuint positions;
    GLuint color;
    GLuint transforms;
  } vbo;

  size_t instance_count;

  size_t attrib_index;
  GLuint vertex_array;

  GLuint index_buffer;
  size_t index_count;
} CmMesh;

CmMesh cm_mesh_create(const uint32_t *indices, size_t indices_count);

void cm_mesh_push_positions(CmMesh *mesh, vec3s *vertices, size_t count);
void cm_mesh_push_colors(CmMesh *mesh, vec4s *colors, size_t count);
void cm_mesh_push_transforms(CmMesh *mesh, mat4s *transforms, size_t count);

void cm_mesh_update_transforms(CmMesh *mesh, mat4s *transforms, size_t count);

void cm_mesh_draw(CmMesh *mesh);
