#ifndef __CM_MESH_H__
#define __CM_MESH_H__

#include "claymore/renderer/render_buffer.h"

typedef struct Transforms Transforms;

typedef struct {
  size_t id;
  bool static_mesh;
  CmRenderBuffer *buffer;
  Transforms *transforms;
} CmMesh;

CmMesh *cm_mesh_create(CmRenderBuffer *buffer, bool static_mesh,
                       size_t transforms_count);
void cm_mesh_delete(CmMesh *mesh);

void cm_mesh_transform_push(CmMesh *mesh, mat4s transform);
void cm_mesh_transform_clear(CmMesh *mesh);

void cm_mesh_draw(CmMesh *mesh);

#endif // __CM_MESH_H__
