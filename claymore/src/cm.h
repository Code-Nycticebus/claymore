#ifndef __CM_DEFINE_H__
#define __CM_DEFINE_H__

#include <stdbool.h>
#include <stdio.h>

#define GLEW_STATIC
#include "GL/glew.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wattributes"
#include "cglm/cglm.h"
#pragma clang diagnostic pop

#include "claymore/debug/debug.h"
#include "claymore/logger/logger.h"

#endif /* !__CM_DEFINE_H__ */
