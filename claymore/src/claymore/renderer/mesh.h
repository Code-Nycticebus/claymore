#pragma once

#include "renderer_defines.h"

typedef struct {
  size_t vertices_count;
  struct {
    GLuint positions;
    GLuint color;
    GLuint uv;
    GLuint normal;
    GLuint transforms;
  } vbo;

  size_t instance_count;

  size_t attrib_index;
  GLuint vertex_array;

  GLuint index_buffer;
  size_t index_count;
} CmMesh;

CmMesh cm_mesh_create(vec3s *vertices, size_t count, const uint32_t *indices,
                      size_t indices_count);

void cm_mesh_attach_colors(CmMesh *mesh, vec4s *colors, size_t count);
void cm_mesh_attach_colors_instanced(CmMesh *mesh, vec4s *colors, size_t count);
void cm_mesh_update_colors(CmMesh *mesh, vec4s *colors, size_t count);

void cm_mesh_attach_normals(CmMesh *mesh, vec3s *normals, size_t count);
void cm_mesh_attach_uv(CmMesh *mesh, vec2s *uvs, size_t count);

void cm_mesh_attach_transforms(CmMesh *mesh, mat4s *transforms, size_t count);

void cm_mesh_update_transforms(CmMesh *mesh, mat4s *transforms, size_t count);

void cm_mesh_draw(CmMesh *mesh);
