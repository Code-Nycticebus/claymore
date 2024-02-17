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

CmVbo cm_mesh_attach_vec3(CmMesh *mesh, usize count, const vec3 *v);
CmVbo cm_mesh_attach_vec3_instanced(CmMesh *mesh, usize count, const vec3 *v);

// u32 cm_mesh_attach_vec4(CmMesh *mesh, usize count, const vec4 *v);
// u32 cm_mesh_attach_vec4_instanced(CmMesh *mesh, usize count, const vec4 *v);

void cm_mesh_draw_indexed(CmMesh *mesh, CmGpuDrawMode mode);
void cm_mesh_draw(CmMesh *mesh, CmGpuDrawMode mode);

#endif /* !__CLAYMORE_MESH_H__ */
