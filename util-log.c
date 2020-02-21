#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#endif

#include "util-log.h"

static const char * l2str[] = {
	"SILENT",
	"ERROR",
	"WARN ",
	"INFO ",
	"DEBUG",
	"TRACE"
};

util_log_callback_t __util_log_callback = util_log_callback_null;
const void *__util_log_callback_arg = NULL;
util_log_level_t __util_log_level = UTIL_LOG_ERROR;

const char *util_log_l2str(const util_log_level_t level) {
	if (level > UTIL_LOG_TRACE) {
		return "N/A";
	}
	else {
		return l2str[level];
	}
}

util_log_level_t util_log_set_level(const util_log_level_t level) {
	util_log_level_t o = __util_log_level;
	__util_log_level = level;
	return o;
}

void util_log_set_callback(const util_log_callback_t callback, const void *arg) {
	__util_log_callback = callback;
	__util_log_callback_arg = arg;
}

void util_log_callback_null(
	__attribute__((unused)) const void *arg,
	__attribute__((unused)) const util_log_level_t level,
	__attribute__((unused)) const char * const prefix,
	__attribute__((unused)) const char * const suffix,
	__attribute__((unused)) const char * const format,
	__attribute__((unused)) va_list args
) {
}

void util_log_callback_fp(
	const void *arg,
	const util_log_level_t level,
	const char * const prefix,
	const char * const suffix,
	const char * const format,
	va_list args
) {
	fprintf((FILE *)arg, "%s %s", util_log_l2str(level), prefix == NULL ? "" : prefix);
	vfprintf((FILE *)arg, format, args);
	fprintf((FILE *)arg, "%s\n", suffix == NULL ? "" : suffix);
}

#ifdef HAVE_SYSLOG_H
static int l2syslog[] = {
	0,
	LOG_ERR,
	LOG_WARNING,
	LOG_INFO,
	LOG_DEBUG,
	LOG_DEBUG
};

void util_log_callback_syslog(
	__attribute__((unused)) const void *arg,
	const util_log_level_t level,
	const char * const prefix,
	const char * const suffix,
	const char * const format,
	va_list args
) {
	char _format[1024];
	util_log_level_t l = level;
	if (l > UTIL_LOG_DEBUG) {
		l = UTIL_LOG_DEBUG; 
	}
	snprintf(
		_format,
		sizeof(_format)-1,
		"%s %s%s%s",
		util_log_l2str(level),
		prefix == NULL ? "" : prefix,
		format,
		suffix == NULL ? "" : suffix
	);
	_format[sizeof(_format)-1] = '\0';
	vsyslog(LOG_USER | l2syslog[l], _format, args);
}
#endif
