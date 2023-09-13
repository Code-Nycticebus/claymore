#ifndef __CM_DEFINE_H__
#define __CM_DEFINE_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define GLEW_STATIC
#include "GL/glew.h"

// Needed because my compile flags give to many warnings when using cglm
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wstatic-in-inline"
#include "cglm/struct.h"
#pragma clang diagnostic pop

#include "claymore/debug/debug.h"
#include "claymore/logger/logger.h"

#endif /* !__CM_DEFINE_H__ */
