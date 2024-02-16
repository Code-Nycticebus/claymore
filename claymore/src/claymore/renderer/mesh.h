#ifndef __CLAYMORE_MESH_H__
#define __CLAYMORE_MESH_H__

#include "claymore/defines.h" // IWYU pragma: export
#include "claymore/renderer/buffer.h"

typedef struct {
  usize vertices_count;
  u32 vertices;

  CmBuffers *buffer;

  usize instance_count;

  usize idx;
  u32 vao;

  u32 ebo;
  usize index_count;
} CmMesh;

CmMesh cm_mesh_create(CmBuffers *b, usize count, const vec3 *vertices);

void cm_mesh_attach_index_buffer(CmMesh *mesh, usize count, const u32 *indices);

void cm_mesh_attach_vec3(CmMesh *mesh, usize count, const vec3 *v);
void cm_mesh_attach_vec3_instanced(CmMesh *mesh, usize count, const vec3 *v);

void cm_mesh_attach_vec4(CmMesh *mesh, usize count, const vec4 *v);
void cm_mesh_attach_vec4_instanced(CmMesh *mesh, usize count, const vec4 *v);

void cm_mesh_draw_indexed(CmMesh *mesh);
void cm_mesh_draw(CmMesh *mesh);

#endif /* !__CLAYMORE_MESH_H__ */
