#ifndef __CLAYMORE_BUFFER_H__
#define __CLAYMORE_BUFFER_H__

#include "claymore/defines.h" // IWYU pragma: export

typedef enum {
  CM_STATIC_DRAW,
  CM_DYNAMIC_DRAW,
} CmGpuType;

typedef struct {
  DA(u32) vbo;
  DA(u32) ebo;
  DA(u32) vao;
} CmGpu;

u32 cm_gpu_vbo(CmGpu *b, CmGpuType type, usize size, const float *v);
void cm_gpu_update_vbo(u32 buffer, usize size, const float *v);

u32 cm_gpu_ebo(CmGpu *b, CmGpuType type, usize count, const u32 *i);
u32 cm_gpu_vao(CmGpu *b);

CmGpu cm_gpu_internal_init(Arena *arena);
void cm_gpu_internal_free(CmGpu *bm);

#endif /* !__CLAYMORE_BUFFER_H__ */
