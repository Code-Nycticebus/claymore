#ifndef __CLAYMORE_GPU_H__
#define __CLAYMORE_GPU_H__

#include "claymore/defines.h" // IWYU pragma: export

/* ========= gpu public ========= */

typedef enum {
  CM_STATIC_DRAW = GL_STATIC_DRAW,
  CM_DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
} CmGpuType;

typedef enum {
  CM_DRAW_TRIANGLES = GL_TRIANGLES,
  CM_DRAW_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
  CM_DRAW_LINES = GL_LINES,
} CmGpuDrawMode;

typedef GLuint CmGpuID;

// buffer struct
typedef struct {
  enum {
    CM_GPU_VBO,
    CM_GPU_EBO,
    CM_GPU_VAO,
    CM_GPU_PROGRAM,
    CM_GPU_TEXTURE,
    CM_GPU_FBO,
    CM_GPU_RBO,
  } type;     // buffer type
  CmGpuID id; // buffer id
} CmGpuBuffer;

/* cpu resource manager context */
typedef struct {
  Arena *arena;            // gpu lifetime arena
  DA(CmGpuBuffer) buffers; // dynamic array of buffer id's
} CmGpu;

/* vertex buffer object */
typedef struct {
  CmGpuID id;
  usize len;
} CmVbo;

// create vbo
CmVbo cm_gpu_vbo(CmGpu *b, CmGpuType type, usize s, usize len, const float *v);
// update vbo buffer
void cm_gpu_vbo_update(CmVbo *vbo, usize s, usize len, const float *v);
// draws the entire vbo
void cm_gpu_vbo_draw(CmVbo *vbo, CmGpuDrawMode mode);
// draws entire vbo many times
void cm_gpu_vbo_draw_instanced(CmVbo *vbo, usize count, CmGpuDrawMode mode);

/* element buffer object */
typedef struct {
  CmGpuID id;
  usize count;
} CmEbo;

// create ebo
CmEbo cm_gpu_ebo(CmGpu *b, CmGpuType type, usize count, const u32 *i);
// draw in specified mode
void cm_gpu_ebo_draw(CmEbo *ebo, usize count, CmGpuDrawMode mode);
// draw instanced in specified mode
void cm_gpu_ebo_draw_instanced(CmEbo *ebo, usize count, CmGpuDrawMode mode);

/* vertex attribute object */
typedef struct {
  CmGpuID id;
  usize idx;
} CmVao;

// create vao
CmVao cm_gpu_vao(CmGpu *b);
// bind vao
void cm_gpu_vao_bind(CmVao *vao);
// pushing attribute
void cm_gpu_vao_push(CmVao *vao, usize count, usize stride, usize offset);
// pushing attribute for instanced rendering
void cm_gpu_vao_instanced(CmVao *vao, usize instance, usize count, usize stride,
                          usize offset);

// create program
CmGpuID cm_gpu_program(CmGpu *b);
// create texture
CmGpuID cm_gpu_texture(CmGpu *b);
// bind texture
void cm_gpu_texture_bind(CmGpuID texture, usize slot);

// create framebuffer
CmGpuID cm_gpu_fbo(CmGpu *b);
// bind framebuffer
void cm_gpu_fbo_bind(CmGpuID fbo);
// create renderbuffer
CmGpuID cm_gpu_rbo(CmGpu *b);
// bind renderbuffer
void cm_gpu_rbo_bind(CmGpuID rbo);

/* ========= gpu internal ========= */

CmGpu cm_gpu_internal_init(Arena *arena);
void cm_gpu_internal_free(CmGpu *bm);

#endif /* !__CLAYMORE_GPU_H__ */
