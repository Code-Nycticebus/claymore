#ifndef __CM_LOGGER_H__
#define __CM_LOGGER_H__

#include <stddef.h>

#if defined(__GNUC__) || defined(__clang__)
#define CM_LOGGER_FORMAT(__fmt_arg, __var_args)                                \
  __attribute__((format(printf, __fmt_arg, __var_args)))
#else
#define CM_LOGGER_FORMAT(...)
#endif

void cm_log_fatal(const char *fmt, ...) CM_LOGGER_FORMAT(1, 2);
void cm_log_error(const char *fmt, ...) CM_LOGGER_FORMAT(1, 2);
void cm_log_warning(const char *fmt, ...) CM_LOGGER_FORMAT(1, 2);
void cm_log_info(const char *fmt, ...) CM_LOGGER_FORMAT(1, 2);

#ifdef CM_DEBUG
void cm_log_debug(const char *fmt, ...) CM_LOGGER_FORMAT(1, 2);
void cm_log_trace(const char *fmt, ...) CM_LOGGER_FORMAT(1, 2);
#else
#define cm_log_debug(...)
#define cm_log_trace(...)
#endif

void cm_logger_init(void);
void cm_logger_destroy(void);

#endif /* !__CM_LOGGER_H__ */
