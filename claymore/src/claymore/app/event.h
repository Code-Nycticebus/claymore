#ifndef __CLAYMORE_EVENTS_H__
#define __CLAYMORE_EVENTS_H__

#include "claymore/defines.h" // IWYU pragma: export

/* ========= event public ========= */

// quit event
typedef struct {
  i32 code; // exit code
} CmEventQuit;

// key pressed or released event
typedef struct {
  RGFW_Key code; // key code
  u32 action;    // press or release
  u32 lock;      // lockstate
} CmEventKey;

// scroll event
typedef struct {
  vec2 offset; // scroll amount (x, y)
} CmEventScroll;

// mouse button press or release
typedef struct {
  u64 button;    // mouse button
  u64 action;    // pressed or released
  vec2 position; // cursor position
} CmEventMouse;

// cursor move event
typedef struct {
  vec2 position;  // cursor position
  vec2 direction; // direction
} CmEventCursor;

// controller button press or release
typedef struct {
  u8 controller; // controller
  vec2 axis[2];  // current joystick axis
  u8 button;     // controller button
  u8 action;     // pressed or released
} CmEventController;

// joystick move
typedef struct {
  u8 controller; // controller
  vec2 axis[2];  // current joystick axis
  u8 button;     // current button pressed
} CmEventJoystick;

// window resize
typedef struct {
  vec2 size; // new size
} CmEventResize;

// file drop event
typedef struct {
  vec2 position;                // drop position
  size_t count;                 // number of files
  char (*files)[RGFW_MAX_PATH]; // file paths
} CmEventDrop;

// window focus
typedef struct {
  bool in_focus; // if window is in focus
} CmEventFocus;

// window move
typedef struct {
  vec2 position; // new position
  vec2 size;     // current size
} CmEventMove;

typedef enum {
  CM_EVENT_QUIT,
  CM_EVENT_KEY,
  CM_EVENT_SCROLL,
  CM_EVENT_MOUSE,
  CM_EVENT_CURSOR,
  CM_EVENT_CONTROLLER,
  CM_EVENT_JOYSTICK,
  CM_EVENT_RESIZE,
  CM_EVENT_DROP,
  CM_EVENT_FOCUS,
  CM_EVENT_MOVE,
} CmEventType;

typedef union {
  CmEventQuit quit;
  CmEventKey key;
  CmEventScroll scroll;
  CmEventMouse mouse;
  CmEventCursor cursor;
  CmEventController controller;
  CmEventJoystick joystick;
  CmEventResize resize;
  CmEventDrop drop;
  CmEventFocus focus;
  CmEventMove move;
} CmEventData;

/* `CmEvent` is a tagged union. It has a tag to identify and
 * a union to store event data.
 * */
typedef struct {
  bool handled;
  CmEventType type;
  CmEventData event;
} CmEvent;

// emits and event
void cm_event_emit(CmEvent event);

/* For each event type, there's a corresponding context macro. To use it, pass
 * the event and your handling code. The macro will check if the event type
 * matches and provide a variable for use in handling code.
 *
 * example:
 *  cm_event_key(event, {
 *    // your key handling code using the 'key' variable
 *  });
 * */

// quit
#define cm_event_quit(e, ...) _cm_event(e, quit, CmEventQuit, CM_EVENT_QUIT, __VA_ARGS__)
// key
#define cm_event_key(e, ...) _cm_event(e, key, CmEventKey, CM_EVENT_KEY, __VA_ARGS__)
// scroll
#define cm_event_scroll(e, ...) _cm_event(e, scroll, CmEventScroll, CM_EVENT_SCROLL, __VA_ARGS__)
// mouse
#define cm_event_mouse(e, ...) _cm_event(e, mouse, CmEventMouse, CM_EVENT_MOUSE, __VA_ARGS__)
// cursor
#define cm_event_cursor(e, ...) _cm_event(e, cursor, CmEventCursor, CM_EVENT_CURSOR, __VA_ARGS__)
// controller
#define cm_event_controller(e, ...)                                                                \
  _cm_event(e, controller, CmEventController, CM_EVENT_CONTROLLER, __VA_ARGS__)
// joystick
#define cm_event_joystick(e, ...)                                                                  \
  _cm_event(e, joystick, CmEventJoystick, CM_EVENT_JOYSTICK, __VA_ARGS__)
// resize
#define cm_event_resize(e, ...) _cm_event(e, resize, CmEventResize, CM_EVENT_RESIZE, __VA_ARGS__)
// drop
#define cm_event_drop(e, ...) _cm_event(e, drop, CmEventDrop, CM_EVENT_DROP, __VA_ARGS__)
// focus
#define cm_event_focus(e, ...) _cm_event(e, focus, CmEventFocus, CM_EVENT_FOCUS, __VA_ARGS__)
// move
#define cm_event_move(e, ...) _cm_event(e, move, CmEventMove, CM_EVENT_MOVE, __VA_ARGS__)

/* ========= event internal ========= */

// helper macro for event context
#define _cm_event(e, MEMBER, T, TYPE, ...)                                                         \
  do {                                                                                             \
    if ((e)->type == TYPE) {                                                                       \
      T *MEMBER = (T *)&(e)->event.MEMBER;                                                         \
      (void)MEMBER;                                                                                \
      __VA_ARGS__                                                                                  \
    }                                                                                              \
  } while (0)

void cm_event_internal_poll_events(RGFW_window *window);

#endif /* !__CLAYMORE_EVENTS_H__ */
