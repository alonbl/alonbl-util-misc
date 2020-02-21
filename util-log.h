#ifndef __UTIL_LOG_H
#define __UTIL_LOG_H

#include <stdarg.h>
#include <stdio.h>

enum util_log_level_e {
	UTIL_LOG_SILENT = 0,
	UTIL_LOG_ERROR,
	UTIL_LOG_WARNING,
	UTIL_LOG_INFO,
	UTIL_LOG_DEBUG,
	UTIL_LOG_TRACE
};
typedef enum util_log_level_e util_log_level_t;

typedef void (*util_log_callback_t)(
	const void *arg,
	const util_log_level_t level,
	const char * const prefix,
	const char * const suffix,
	const char * const format,
	va_list args
);

extern util_log_callback_t __util_log_callback;
extern const void * __util_log_callback_arg;
extern util_log_level_t __util_log_level;

const char *util_log_l2str(const util_log_level_t level);

static
inline
__attribute__((unused))
util_log_level_t
util_log_get_level(void) {
	return __util_log_level;
}

void util_log_set_callback(const util_log_callback_t callback, const void *arg);

util_log_level_t util_log_set_level(const util_log_level_t level);

static
inline
void
__attribute__((unused))
__attribute__((format (printf, 4, 0)))
__util_log_invoke_callback(
	const util_log_level_t level,
	const char * const prefix,
	const char * const suffix,
	const char * const format,
	va_list args
) {
	__util_log_callback(__util_log_callback_arg, level, prefix, suffix, format, args);
}

static
inline
void
__attribute__((unused))
__attribute__((format (printf, 4, 0)))
util_log_vprintf(
	const util_log_level_t level,
	const char * const prefix,
	const char * const suffix,
	const char * const format,
	va_list args
) {
	if (level <= util_log_get_level()) {
		__util_log_invoke_callback(level, prefix, suffix, format, args);
	}
}

static
inline
void
__attribute__((unused))
__attribute__((format (printf, 3, 0)))
util_log_printf_f(
	const util_log_level_t level,
	const char * const prefix,
	const char * const suffix,
	const char * const format,
	...
) {
	if (level <= util_log_get_level()) {
		va_list args;
		va_start(args, format);
		__util_log_invoke_callback(level, prefix, suffix, format, args);
		va_end(args);
	}
}

static
inline
void
__attribute__((unused))
__attribute__((format (printf, 2, 3)))
util_log_printf(
	const util_log_level_t level,
	const char * const format,
	...
) {
	if (level <= util_log_get_level()) {
		va_list args;
		va_start(args, format);
		__util_log_invoke_callback(level, NULL, NULL, format, args);
		va_end(args);
	}
}

void util_log_callback_null(
	const void *arg,
	const util_log_level_t level,
	const char * const prefix,
	const char * const suffix,
	const char * const format,
	va_list args
);

void util_log_callback_fp(
	const void *arg,
	const util_log_level_t level,
	const char * const prefix,
	const char * const suffix,
	const char * const format,
	va_list args
);

#ifdef HAVE_SYSLOG_H
void util_log_callback_syslog(
	const void *arg,
	const util_log_level_t level,
	const char * const prefix,
	const char * const suffix,
	const char * const format,
	va_list args
);
#endif

#endif
