#ifndef __CLAYMORE_MESH_H__
#define __CLAYMORE_MESH_H__

#include "claymore/defines.h" // IWYU pragma: export
#include "claymore/renderer/gpu.h"

typedef struct {
  CmGpu *buffer;

  usize count;

  CmVbo vbo;
  CmVao vao;
  CmEbo ebo;
} CmMesh;

CmMesh cm_mesh_create(CmGpu *b, usize count, const vec3 *vertices);

void cm_mesh_attach_ebo(CmMesh *mesh, usize count, const u32 *indices);

CmVbo cm_mesh_attach_f32(CmMesh *mesh, usize count, const f32 *v);
CmVbo cm_mesh_attach_f32_instanced(CmMesh *mesh, usize count, const f32 *v);

CmVbo cm_mesh_attach_vec2(CmMesh *mesh, usize count, const vec2 *v);
CmVbo cm_mesh_attach_vec2_instanced(CmMesh *mesh, usize count, const vec2 *v);

CmVbo cm_mesh_attach_vec3(CmMesh *mesh, usize count, const vec3 *v);
CmVbo cm_mesh_attach_vec3_instanced(CmMesh *mesh, usize count, const vec3 *v);

void cm_mesh_draw_indexed(CmMesh *mesh, CmGpuDrawMode mode);
void cm_mesh_draw(CmMesh *mesh, CmGpuDrawMode mode);

#endif /* !__CLAYMORE_MESH_H__ */
