#include "key.h"

#include <assert.h>

static CmKeyAction key_state[CM_KEY_COUNT] = {0};

void cm_key_set(CmKeyCodes key, CmKeyAction action) {
  assert(key > 0 && key < CM_KEY_COUNT);
  key_state[key] = action;
}

CmKeyAction cm_key(CmKeyCodes key) {
  assert(key > 0 && key < CM_KEY_COUNT);
  return key_state[key];
}
