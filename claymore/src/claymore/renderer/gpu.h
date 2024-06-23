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

typedef u32 CmGpuID;

typedef struct {
  CmGpuID id;
  usize len;
} CmVbo;

typedef struct {
  CmGpuID id;
  usize count;
} CmEbo;

typedef struct {
  CmGpuID id;
  CmGpuID idx;
} CmVao;

// Buffer struct
typedef struct {
  enum {
    CM_GPU_VBO,
    CM_GPU_EBO,
    CM_GPU_VAO,
    CM_GPU_PROGRAM,
    CM_GPU_TEXTURE,
  } type;     // Buffer type
  CmGpuID id; // Buffer id
} CmGpuBuffer;

/* Cpu resource manager context */
typedef struct {
  Arena *arena;            // Gpu lifetime arena
  DA(CmGpuBuffer) buffers; // Dynamic array of buffers
} CmGpu;

// Create vbo
CmVbo cm_gpu_vbo(CmGpu *b, CmGpuType type, usize s, usize len, const float *v);
// Update vbo buffer
void cm_gpu_vbo_update(CmVbo *vbo, usize s, usize len, const float *v);
// Draw vbo instanced
void cm_gpu_vbo_draw_instanced(CmVbo *vbo, usize count, CmGpuDrawMode mode);

// Create ebo
CmEbo cm_gpu_ebo(CmGpu *b, CmGpuType type, usize count, const u32 *i);
// Draw in specified mode
void cm_gpu_ebo_draw(CmEbo *ebo, usize count, CmGpuDrawMode mode);
// Draw instanced in specified mode
void cm_gpu_ebo_draw_instanced(CmEbo *ebo, usize count, CmGpuDrawMode mode);

// Create vao
CmVao cm_gpu_vao(CmGpu *b);
// Bind vao
void cm_gpu_vao_bind(CmVao *vao);
// Pushing attribute
void cm_gpu_vao_push(CmVao *vao, usize count, usize stride, usize offset);
// Pushing attribute for instanced rendering
void cm_gpu_vao_instanced(CmVao *vao, usize instance, usize count, usize stride,
                          usize offset);

// Create program
CmGpuID cm_gpu_program(CmGpu *b);
// Create texture
CmGpuID cm_gpu_texture(CmGpu *b);

CmGpu cm_gpu_internal_init(Arena *arena);
void cm_gpu_internal_free(CmGpu *bm);

#endif /* !__CLAYMORE_GPU_H__ */
