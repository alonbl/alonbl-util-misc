#include <string.h>

#include "util-log.h"

static struct buf_s {
	util_log_level_t level;
	char msg[1024];
} buf;

void util_log_callback_buf(
	const void *arg,
	const util_log_level_t level,
	__attribute__((unused)) const char * const prefix,
	__attribute__((unused)) const char * const suffix,
	const char * const format,
	va_list args
) {
	struct buf_s *b = (struct buf_s *)arg;
	b->level = level;
	vsnprintf(b->msg, sizeof(b->msg), format, args);
}

void assert_reset(void) {
	buf.level = UTIL_LOG_SILENT;
	buf.msg[0] = '\0';
}

int assert_buf(const char * const error, const util_log_level_t level, const char * const msg) {
	int r = level == buf.level && strcmp(msg, buf.msg) == 0;
	if (!r) {
		printf("ERROR: %s\n", error);
	}
	return r;
}

int main(void) {
	int ret = 1;

	util_log_set_callback(util_log_callback_buf, &buf);

	util_log_set_level(UTIL_LOG_INFO);
	assert_reset();
	util_log_printf(UTIL_LOG_DEBUG, "hello");
	if (!assert_buf("Should be empty", UTIL_LOG_SILENT, "")) {
		goto cleanup;
	}
	assert_reset();
	util_log_printf(UTIL_LOG_INFO, "hello");
	if (!assert_buf("Should be hello info", UTIL_LOG_INFO, "hello")) {
		goto cleanup;
	}
	assert_reset();
	util_log_printf(UTIL_LOG_ERROR, "hello1");
	if (!assert_buf("Should be hello error", UTIL_LOG_ERROR, "hello1")) {
		goto cleanup;
	}

	ret = 0;

cleanup:
	return ret;
}
