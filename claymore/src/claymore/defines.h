#ifndef __CLAYMORE_DEFINES_H__
#define __CLAYMORE_DEFINES_H__

#include "cglm/cglm.h" // IWYU pragma: export
#include "clib/clib.h" // IWYU pragma: export

#define VEC2_FMT "%f, %f"
#define VEC2_ARG(v) v[0], v[1]

#define VEC3_FMT "%f, %f, %f"
#define VEC3_ARG(v) v[0], v[1], v[2]

#define VEC4_FMT "%f, %f, %f, %f"
#define VEC4_ARG(v) v[0], v[1], v[2], v[3]

#ifndef NDEBUG
#define _CM_DEBUG
#endif

#endif /* !__CLAYMORE_DEFINES_H__ */
