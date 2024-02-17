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

u32 cm_buffer_vbo(CmBuffers *b, CmBufferType type, usize size, const float *v);
void cm_buffer_update_vbo(u32 buffer, usize size, const float *v);

u32 cm_buffer_ebo(CmBuffers *b, CmBufferType type, usize count, const u32 *i);
u32 cm_buffer_vao(CmBuffers *b);

CmBuffers cm_buffer_internal_init(Arena *arena);
void cm_buffer_internal_free(CmBuffers *bm);

#endif /* !__CLAYMORE_BUFFER_H__ */
