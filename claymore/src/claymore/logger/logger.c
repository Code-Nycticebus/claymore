#include "cm.h"
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

typedef enum {
  CM_LOG_FATAL = 0,
  CM_LOG_ERROR,
  CM_LOG_WARN,
  CM_LOG_INFO,
  CM_LOG_DEBUG,
  CM_LOG_TRACE,
} CmLogLevel;

#define FMT_RESET "\033[0m"

#define _LOG(__log_level, __fmt)                                               \
  char __buffer[CM_LOGGER_CHAR_BUFFER_SIZE];                                   \
  va_list __args;                                                              \
  va_start(__args, __fmt);                                                     \
  size_t __size =                                                              \
      vsnprintf(__buffer, CM_LOGGER_CHAR_BUFFER_SIZE - 1, __fmt, __args);      \
  va_end(__args);                                                              \
  cm_log(__log_level, __size, __buffer);

static void cm_log(CmLogLevel log_level, size_t size, const char *msg) {
  static const struct CmLogLevelPrefix log_level_str[] = {
      [CM_LOG_FATAL] = {"FATAL", "\033[1m\033[91m"},
      [CM_LOG_ERROR] = {"ERROR", "\033[91m"},
      [CM_LOG_WARN] = {"WARN", "\033[33m"},
      [CM_LOG_INFO] = {"INFO", "\033[34m"},
      [CM_LOG_DEBUG] = {"DEBUG", "\033[1m\033[95m"},
      [CM_LOG_TRACE] = {"TRACE", "\033[97m"},
  };

  (void)size;
  fprintf(stdout, "%s[CLAYMORE][%s]: %s" FMT_RESET,
          log_level_str[log_level].color, log_level_str[log_level].prefix, msg);
}

void cm_log_fatal(const char *fmt, ...) { _LOG(CM_LOG_FATAL, fmt); }
void cm_log_error(const char *fmt, ...) { _LOG(CM_LOG_ERROR, fmt); }
void cm_log_warning(const char *fmt, ...) { _LOG(CM_LOG_WARN, fmt); }
void cm_log_info(const char *fmt, ...) { _LOG(CM_LOG_INFO, fmt); }

#ifdef _CM_DEBUG
void cm_log_debug(const char *fmt, ...) { _LOG(CM_LOG_DEBUG, fmt); }
void cm_log_trace(const char *fmt, ...) { _LOG(CM_LOG_TRACE, fmt); }
#endif

void cm_logger_init(void) {}
void cm_logger_destroy(void) {}
