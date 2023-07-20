#ifndef __CM_KEY_H__
#define __CM_KEY_H__

#include "claymore/events/keycodes.h"

#include "event_types.h"

CmKeyAction cm_key(CmKeyCodes key);
void cm_key_set(CmKeyCodes key, CmKeyAction action);

#endif /* !__CM_KEY_H__ */
