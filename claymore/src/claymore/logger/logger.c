#define _CM_LOGGER_INTERNAL
#include "logger.h"

#include <stdarg.h>
#include <stdio.h>

static struct {
  CmLoggerFn out;
  CmLoggerFn err;
} logger;

#define CM_LOGGER_CHAR_BUFFER_SIZE 1000

void cm_log_dbg(const char *fmt, ...) {
  char fmt_buffer[CM_LOGGER_CHAR_BUFFER_SIZE] = {0};
  va_list args;
  va_start(args, fmt);
  vsnprintf(fmt_buffer, CM_LOGGER_CHAR_BUFFER_SIZE, fmt, args);
  va_end(args);
  logger.out.log_function(logger.out.log_file, fmt_buffer);
}

void cm_log_err(const char *fmt, ...) {
  char fmt_buffer[CM_LOGGER_CHAR_BUFFER_SIZE] = {0};
  va_list args;
  va_start(args, fmt);
  vsnprintf(fmt_buffer, CM_LOGGER_CHAR_BUFFER_SIZE, fmt, args);
  va_end(args);
  logger.out.log_function(logger.out.log_file, fmt_buffer);
}

void cm_logger_init(CmLoggerFn out, CmLoggerFn err) {
  logger.out =
      out.log_file == NULL ? (CmLoggerFn){(cm_log_fn)fprintf, stdout} : out;
  logger.err =
      err.log_file == NULL ? (CmLoggerFn){(cm_log_fn)fprintf, stderr} : err;
}
void cm_logger_destroy(void) {}
