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

#define CM_FATAL(...) cm_log(CM_LOG_FATAL, __VA_ARGS__)
#define CM_ERROR(...) cm_log(CM_LOG_ERROR, __VA_ARGS__)
#define CM_WARN(...) cm_log(CM_LOG_WARN, __VA_ARGS__)
#define CM_INFO(...) cm_log(CM_LOG_INFO, __VA_ARGS__)

#ifdef _CM_DEBUG
#define CM_DEBUG(...) cm_log(CM_LOG_DEBUG, __VA_ARGS__)
#define CM_TRACE(...) cm_log(CM_LOG_TRACE, __VA_ARGS__)
#else
#define CM_DEBUG(...)
#define CM_TRACE(...)
#endif

void cm_log(CmLogLevel log_level, const char *fmt, ...) CM_LOGGER_FORMAT(2, 3);

#ifdef _CM_LOGGER_INTERNAL
void cm_logger_init(void);
void cm_logger_destroy(void);
#endif // !_CM_LOGGER_INTERNAL

#endif /* !__CM_LOGGER_H__ */
