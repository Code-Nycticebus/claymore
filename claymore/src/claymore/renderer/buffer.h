#ifndef __CLAYMORE_BUFFER_H__
#define __CLAYMORE_BUFFER_H__

#include "claymore/defines.h" // IWYU pragma: export

typedef enum {
  CM_STATIC_DRAW,
  CM_DYNAMIC_DRAW,
} CmBufferType;

typedef struct {
  DA(u32) vbo;
  DA(u32) ebo;
  DA(u32) vao;
} CmBuffers;

u32 cm_buffer_vbo(CmBuffers *b, CmBufferType type, const float *v, usize size);
u32 cm_buffer_ebo(CmBuffers *b, CmBufferType type, const u32 *i, usize count);
u32 cm_buffer_vao(CmBuffers *b);

CmBuffers cm_buffer_init(Arena *arena);
void cm_buffer_free(CmBuffers *bm);

#endif /* !__CLAYMORE_BUFFER_H__ */
