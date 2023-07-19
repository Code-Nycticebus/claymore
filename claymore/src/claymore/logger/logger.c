#define _CM_LOGGER_INTERNAL
#include "logger.h"

#include <stdarg.h>
#include <stdio.h>

static struct {
  cm_log_file *out;
  cm_log_file *err;
} logger;

#define CM_LOGGER_VFPRINTF(__out_file__)                                       \
  va_list args;                                                                \
  va_start(args, fmt);                                                         \
  vfprintf(__out_file__, fmt, args);                                           \
  va_end(args)

void cm_log_dbg(const char *fmt, ...) { CM_LOGGER_VFPRINTF(logger.out); }

void cm_log_err(const char *fmt, ...) { CM_LOGGER_VFPRINTF(logger.err); }

void cm_logger_init(cm_log_file *out, cm_log_file *err) {
  logger.out = out == NULL ? stdout : out;
  logger.err = err == NULL ? stderr : err;
}
void cm_logger_destroy(void) {}
