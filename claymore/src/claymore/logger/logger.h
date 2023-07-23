#ifndef __CM_LOGGER_H__
#define __CM_LOGGER_H__

#include <stddef.h>

#ifdef __GNUC__
#define CM_LOGGER_FORMAT(__fmt_arg, __var_args)                                \
  __attribute__((format(printf, __fmt_arg, __var_args)))
#else
#define CM_LOGGER_FORMAT(...)
#endif

typedef enum {
  CM_LOG_FATAL = 0,
  CM_LOG_ERROR,
  CM_LOG_WARN,
  CM_LOG_INFO,
  CM_LOG_DEBUG,
  CM_LOG_TRACE,
} CmLogLevel;

#define CM_FATAL(msg, ...) cm_log(CM_LOG_FATAL, msg, __VA_ARGS__)
#define CM_ERROR(msg, ...) cm_log(CM_LOG_ERROR, msg, __VA_ARGS__)
#define CM_WARN(msg, ...) cm_log(CM_LOG_WARN, msg, __VA_ARGS__)
#define CM_INFO(msg, ...) cm_log(CM_LOG_INFO, msg, __VA_ARGS__)

#ifdef _CM_DEBUG
#define CM_DEBUG(msg, ...) cm_log(CM_LOG_DEBUG, msg, __VA_ARGS__)
#define CM_TRACE(msg, ...) cm_log(CM_LOG_TRACE, msg, __VA_ARGS__)
#else
#define CM_DEBUG(msg, ...)
#define CM_TRACE(msg, ...)
#endif

void cm_log(CmLogLevel log_level, const char *fmt, ...) CM_LOGGER_FORMAT(2, 3);

#ifdef _CM_LOGGER_INTERNAL
void cm_logger_init(void);
void cm_logger_destroy(void);
#endif // !_CM_LOGGER_INTERNAL

#endif /* !__CM_LOGGER_H__ */
