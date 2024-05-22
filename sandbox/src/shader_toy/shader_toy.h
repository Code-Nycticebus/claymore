#ifndef __SANDBOX_SHADER_H__
#define __SANDBOX_SHADER_H__

#include "claymore.h"

CmScene *shader_init(CmScene *parent, Str filename, Error *error);
CmScene *error_display_init(CmScene *parent, Str message);

#endif /* !__SANDBOX_SHADER_H__ */
