#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <unistd.h>
#include <fcntl.h>
#endif

#include "util-pass.h"

int mystrcmp(const char * const a, const char * const b) {
	if (a == NULL && b == NULL) {
		return 0;
	}
	else if (a == NULL || b == NULL) {
		return 1;
	}
	else {
		return strcmp(a, b);
	}
}

int main() {
	char password[1024];
#ifndef _WIN32
	char buf[10];
	int fd;
#endif
	int ret = 1;

	if (util_pass_get("xxx:xyz", password, sizeof(password)) != NULL) {
		printf("error wrong type");
		goto cleanup;
	}

	if (mystrcmp(util_pass_get("pass:xyz", password, sizeof(password)), "xyz")) {
		printf("error pass");
		goto cleanup;
	}
	
	putenv("xyz=aaa");
	if (mystrcmp(util_pass_get("env:xyz", password, sizeof(password)), "aaa")) {
		printf("error env:1");
		goto cleanup;
	}
	if (util_pass_get("env:xyza", password, sizeof(password)) != NULL) {
		printf("error env:2");
		goto cleanup;
	}

	if (mystrcmp(util_pass_get("file:password.txt", password, sizeof(password)), "line1")) {
		printf("error file");
		goto cleanup;
	}
	
#ifndef _WIN32
	fd = open("password.txt", O_RDONLY);
	snprintf(buf, sizeof(buf), "fd:%d", fd);
	if (mystrcmp(util_pass_get(buf, password, sizeof(password)), "line1")) {
		printf("error fd:1");
		goto cleanup;
	}
	if (mystrcmp(util_pass_get(buf, password, sizeof(password)), "line2")) {
		printf("error fd:2");
		goto cleanup;
	}
	close(fd);
#endif

	ret = 0;

cleanup:

	return ret;
}
