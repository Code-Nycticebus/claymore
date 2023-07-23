#define _CM_LOGGER_INTERNAL
#include "logger.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#define CM_LOGGER_CHAR_BUFFER_SIZE 1000

struct CmLogLevelPrefix {
  const char *prefix;
  const char *color;
};

#define FMT_RESET "\033[0m"

void cm_log(CmLogLevel log_level, const char *fmt, ...) {
  static const struct CmLogLevelPrefix log_level_str[] = {
      [CM_LOG_FATAL] = {"FATAL", "\033[31m"},
      [CM_LOG_ERROR] = {"ERROR", "\033[31m"},
      [CM_LOG_WARN] = {"WARN", "\033[33m"},
      [CM_LOG_INFO] = {"INFO", "\033[32m"},
      [CM_LOG_DEBUG] = {"DEBUG", "\033[37m"},
      [CM_LOG_TRACE] = {"TRACE", "\033[37m"},
  };

  char fmt_buffer[CM_LOGGER_CHAR_BUFFER_SIZE] = {0};
  va_list args;
  va_start(args, fmt);
  vsnprintf(fmt_buffer, CM_LOGGER_CHAR_BUFFER_SIZE, fmt, args);
  va_end(args);

  fprintf(stdout, "%s[CLAYMORE][%s]: %s" FMT_RESET "\n",
          log_level_str[log_level].color, log_level_str[log_level].prefix,
          fmt_buffer);
}

void cm_logger_init(void) {}
void cm_logger_destroy(void) {}
