#ifndef __CLAYMORE_GPU_H__
#define __CLAYMORE_GPU_H__

#include "claymore/defines.h" // IWYU pragma: export

typedef enum {
  CM_STATIC_DRAW = GL_STATIC_DRAW,
  CM_DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
} CmGpuType;

typedef enum {
  CM_DRAW_TRIANGLES = GL_TRIANGLES,
  CM_DRAW_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
  CM_DRAW_LINES = GL_LINES,
} CmGpuDrawMode;

typedef struct {
  u32 id;
  usize len;
} CmVbo;

typedef struct {
  u32 id;
  usize count;
} CmEbo;

typedef struct {
  u32 id;
  u32 idx;
} CmVao;

typedef u32 CmGpuID;

typedef struct {
  enum {
    CM_GPU_VBO,
    CM_GPU_EBO,
    CM_GPU_VAO,
    CM_GPU_PROGRAM,
    CM_GPU_TEXTURE,
  } type;
  CmGpuID id;
} CmGpuBuffer;

typedef struct {
  Arena *arena;
  DA(CmGpuBuffer) buffers;
} CmGpu;

CmVbo cm_gpu_vbo(CmGpu *b, CmGpuType type, usize s, usize len, const float *v);
void cm_gpu_vbo_update(CmVbo *vbo, usize s, usize len, const float *v);
void cm_gpu_vbo_draw_instanced(CmVbo *vbo, usize count, CmGpuDrawMode mode);

CmEbo cm_gpu_ebo(CmGpu *b, CmGpuType type, usize count, const u32 *i);
void cm_gpu_ebo_draw(CmEbo *ebo, usize count, CmGpuDrawMode mode);
void cm_gpu_ebo_draw_instanced(CmEbo *ebo, usize count, CmGpuDrawMode mode);

CmVao cm_gpu_vao(CmGpu *b);
void cm_gpu_vao_bind(CmVao *vao);
void cm_gpu_vao_push(CmVao *vao, usize count, usize stride, usize offset);
void cm_gpu_vao_instanced(CmVao *vao, usize instance, usize count, usize stride,
                          usize offset);

CmGpuID cm_gpu_program(CmGpu *b);
CmGpuID cm_gpu_texture(CmGpu *b);

CmGpu cm_gpu_internal_init(Arena *arena);
void cm_gpu_internal_free(CmGpu *bm);

#endif /* !__CLAYMORE_GPU_H__ */
