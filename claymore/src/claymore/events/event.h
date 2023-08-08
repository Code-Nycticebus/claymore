#ifndef __CM_EVENTS_H__
#define __CM_EVENTS_H__

#include "event_types.h"
#include "key.h"
#include "keycodes.h"
#include "mouse.h"
#include <stdbool.h>

typedef void (*cm_event_callback)(void *data, CmEventUnion *event);

void cm_event_dispatch(CmEvent event);
void cm_event_set_callback(void *data, CmEventType type,
                           cm_event_callback callback);

#endif /* !__CM_EVENTS_H__ */
