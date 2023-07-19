#ifndef __CM_KEY_H__
#define __CM_KEY_H__

#include "claymore/events/keycodes.h"

#include <stdint.h>

typedef enum {
  CM_KEY_RELEASE = 0,
  CM_KEY_PRESS,
  CM_KEY_REPEAT,
} CmKeyAction;

typedef struct {
  CmKeyAction action;
  CmKeyCodes code;
} CmKeyEvent;

CmKeyAction cm_key(CmKeyCodes key);
void cm_key_set(CmKeyCodes key, CmKeyAction action);

#endif /* !__CM_KEY_H__ */
