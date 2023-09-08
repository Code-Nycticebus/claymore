#ifndef __CM_APP_H__
#define __CM_APP_H__

#include "app_defines.h"

bool cm_app_init(CmApp *app, const ClaymoreConfig *config);
void cm_app_shutdown(CmApp *app, const ClaymoreConfig *config);

void cm_app_run(CmApp *app);

/* User functions! */
extern ClaymoreConfig claymore_config(void);

#endif /* !__CM_APP_H__ */
