#ifndef __CLAYMORE_MESH_H__
#define __CLAYMORE_MESH_H__

#include "claymore/defines.h" // IWYU pragma: export

typedef struct {
  usize vertices_count;
  u32 vertices;

  DA(u32) vbo;

  usize instance_count;

  usize idx;
  u32 vao;

  u32 ebo;
  usize index_count;
} CmMesh;

CmMesh cm_mesh_create(Arena *arena, const vec3 *vertices, usize count);
void cm_mesh_delete(CmMesh *mesh);

void cm_mesh_attach_index_buffer(CmMesh *mesh, const u32 *indices, usize count);

void cm_mesh_attach_vec3(CmMesh *mesh, const vec3 *v, usize count);
void cm_mesh_attach_vec3_instanced(CmMesh *mesh, const vec3 *v, usize count);

void cm_mesh_draw_indexed(CmMesh *mesh);
void cm_mesh_draw(CmMesh *mesh);

#endif /* !__CLAYMORE_MESH_H__ */
