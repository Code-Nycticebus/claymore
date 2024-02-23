#include "mesh.h"

#include "claymore/renderer/gpu.h"

CmMesh cm_mesh_create(CmGpu *b, usize count, const vec3 *vertices) {
  CmMesh mesh = {0};
  mesh.count = 1;
  mesh.buffer = b;

  mesh.vbo =
      cm_gpu_vbo(b, CM_STATIC_DRAW, sizeof(vec3), count, &vertices[0][0]);

  mesh.vao = cm_gpu_vao(b);
  cm_gpu_vao_push(&mesh.vao, 3, sizeof(vec3), 0);

  return mesh;
}

void cm_mesh_attach_ebo(CmMesh *mesh, usize count, const u32 *indices) {
  cm_gpu_vao_bind(&mesh->vao);
  mesh->ebo = cm_gpu_ebo(mesh->buffer, CM_STATIC_DRAW, count, indices);
}

CmVbo cm_mesh_attach_f32(CmMesh *mesh, usize count, const f32 *v) {
  clib_assert(mesh->vbo.len == count, "This would result in a crash");

  CmVbo vbo =
      cm_gpu_vbo(mesh->buffer, CM_DYNAMIC_DRAW, sizeof(float), count, &v[0]);
  cm_gpu_vao_push(&mesh->vao, 1, sizeof(f32), 0);
  return vbo;
}

CmVbo cm_mesh_attach_f32_instanced(CmMesh *mesh, usize count, const f32 *v) {
  clib_assert(mesh->count == 1 || mesh->count == count,
              "This would result in a crash");
  mesh->count = count;

  CmVbo vbo =
      cm_gpu_vbo(mesh->buffer, CM_DYNAMIC_DRAW, sizeof(float), count, &v[0]);
  cm_gpu_vao_instanced(&mesh->vao, 1, 1, sizeof(f32), 0);
  return vbo;
}

CmVbo cm_mesh_attach_vec2(CmMesh *mesh, usize count, const vec2 *v) {
  clib_assert(mesh->vbo.len == count, "This would result in a crash");

  CmVbo vbo =
      cm_gpu_vbo(mesh->buffer, CM_DYNAMIC_DRAW, sizeof(vec2), count, &v[0][0]);
  cm_gpu_vao_push(&mesh->vao, 2, sizeof(vec2), 0);
  return vbo;
}

CmVbo cm_mesh_attach_vec2_instanced(CmMesh *mesh, usize count, const vec2 *v) {
  clib_assert(mesh->count == 1 || mesh->count == count,
              "This would result in a crash");
  mesh->count = count;

  CmVbo vbo =
      cm_gpu_vbo(mesh->buffer, CM_DYNAMIC_DRAW, sizeof(vec2), count, &v[0][0]);

  cm_gpu_vao_instanced(&mesh->vao, 1, 2, sizeof(vec2), 0);
  return vbo;
}

CmVbo cm_mesh_attach_vec3(CmMesh *mesh, usize count, const vec3 *v) {
  clib_assert(mesh->vbo.len == count, "This would result in a crash");

  CmVbo vbo =
      cm_gpu_vbo(mesh->buffer, CM_DYNAMIC_DRAW, sizeof(vec3), count, &v[0][0]);
  cm_gpu_vao_push(&mesh->vao, 3, sizeof(vec3), 0);
  return vbo;
}

CmVbo cm_mesh_attach_vec3_instanced(CmMesh *mesh, usize count, const vec3 *v) {
  clib_assert(mesh->count == 1 || mesh->count == count,
              "This would result in a crash");
  mesh->count = count;

  CmVbo vbo =
      cm_gpu_vbo(mesh->buffer, CM_DYNAMIC_DRAW, sizeof(vec3), count, &v[0][0]);

  cm_gpu_vao_instanced(&mesh->vao, 1, 3, sizeof(vec3), 0);
  return vbo;
}

void cm_mesh_draw_indexed(CmMesh *mesh, CmGpuDrawMode mode) {
  cm_gpu_vao_bind(&mesh->vao);
  cm_gpu_ebo_draw_instanced(&mesh->ebo, mesh->count, mode);
}

void cm_mesh_draw(CmMesh *mesh, CmGpuDrawMode mode) {
  cm_gpu_vao_bind(&mesh->vao);
  cm_gpu_vbo_draw_instanced(&mesh->vbo, mesh->count, mode);
}
