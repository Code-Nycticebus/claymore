#ifndef __CM_LOGGER_H__
#define __CM_LOGGER_H__

#include <stddef.h>

typedef void cm_log_file;

#ifdef __GNUC__
#define CM_LOGGER_FORMAT(__fmt_arg, __var_args)                                \
  __attribute__((format(printf, __fmt_arg, __var_args)))
#else
#define CM_LOGGER_FORMAT(...)
#endif

void cm_log_dbg(const char *fmt, ...) CM_LOGGER_FORMAT(1, 2);
void cm_log_err(const char *fmt, ...) CM_LOGGER_FORMAT(1, 2);

#ifdef _CM_LOGGER_INTERNAL
void cm_logger_init(cm_log_file *out, cm_log_file *err);
void cm_logger_destroy(void);
#endif // !_CM_LOGGER_INTERNAL

#endif /* !__CM_LOGGER_H__ */
