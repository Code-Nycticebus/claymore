#ifndef __CM_RENDERER_DEFINES_H__
#define __CM_RENDERER_DEFINES_H__

#include "cm.h"

typedef struct {
  vec3s pos;
  vec4s color;
  vec2s uv;
} CmVertex2D;

typedef struct {
  vec3s pos;
  vec4s color;
  vec3s normal;
  vec2s uv;
} CmVertex3D;

#endif /* !__CM_RENDERER_DEFINES_H__ */
