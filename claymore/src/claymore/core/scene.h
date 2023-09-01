#ifndef __CM_SCENE_H__
#define __CM_SCENE_H__

#include "app_defines.h"

bool cm_scene_init(CmApp *app, const ClaymoreConfig *config);
void cm_scene_update(float dt);
void cm_scene_free(void);

void cm_scene_change(size_t scene);

#endif /* !__CM_SCENE_H__ */
