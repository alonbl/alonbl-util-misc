#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <unistd.h>
#endif

#include "util-pass.h"

static int myread(
	FILE *fp,
	char * const password,
	const size_t size
) {
	char * p = password;
	size_t n = size;
	char c;
	int found = 0;

	while (!found && n > 0 && fread(&c, 1, 1, fp) == 1) {
		if (c == '\n' || c == '\r') {
			*p = '\0';
			found = 1;
			break;
		}
		else {
			*p++ = c;
			n--;
		}
	}

	return found;
}

char * util_pass_get(
	const char * const arg,
	char * const password,
	const size_t size
) {
	const char *p;
	size_t n;
	int ret = 0;

	if (password == NULL || size < 1) {
		goto cleanup;
	}

	if ((p = strchr(arg, ':')) == NULL) {
		goto cleanup;
	}
	n = p - arg;
	p++;

	if (!strncmp(arg, "pass", n)) {
		if (strlen(p) >= size) {
			goto cleanup;
		}
		strcpy(password, p);
		ret = 1;
	}
	else if (!strncmp(arg, "env", n)) {
		if ((p = getenv(p)) == NULL) {
			goto cleanup;
		}
		if (strlen(p) >= size) {
			goto cleanup;
		}
		strcpy(password, p);
		ret = 1;
	}
	else if (!strncmp(arg, "file", n)) {
		FILE *fp;
		if ((fp = fopen(p, "r")) != NULL) {
			ret = myread(fp, password, size);
			fclose(fp);
		}
	}
#ifndef _WIN32
	else if (!strncmp(arg, "fd", n)) {
		FILE *fp;
		int fd;

		if ((fd = dup(atoi(p))) != -1) {
			if ((fp = fdopen(fd, "r")) != NULL) {
				char c;
				setbuffer(fp, &c, sizeof(c));
				ret = myread(fp, password, size);
				fclose(fp);
			}
			else {
				close(fd);
			}
		}
	}
#endif
	else if (!strncmp(arg, "stdin", n)) {
		ret = myread(stdin, password, size);
	}

cleanup:

	return ret ? password : NULL;
}
