#ifndef __CM_WINDOW_H__
#define __CM_WINDOW_H__

#include "cm.h"

typedef struct GLFWwindow CMwindow;

CMwindow *cm_window_init(uint32_t width, uint32_t height, const char *name);
void cm_window_input_handler(CMwindow *window);
float cm_window_time(void);

#endif /* !__CM_WINDOW_H__ */
