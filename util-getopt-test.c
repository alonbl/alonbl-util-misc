#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util-getopt.h"

int main(int argc, char *argv[]) {
	const struct option long_options[] = {
		{
			"help\0usage",
			no_argument,
			NULL,
			'h'
		},
		{
			"arg1\0arg1 desc",
			required_argument,
			NULL,
			'1'
		},
		{
			"arg2\0arg2 desc",
			optional_argument,
			NULL,
			'2'
		},
		{
			"arg3\0arg3 desc",
			optional_argument,
			NULL,
			0x200
		},
		{
			"arg4\0arg4 desc",
			required_argument,
			NULL,
			'4'
		},
		{
			NULL,
			0,
			NULL,
			0
		}
	};
	char optstring[1024];
	int opt;
	int ret = 1;

	if (util_getopt_short_from_long(long_options, optstring, 5)) {
		goto cleanup;
	}

	if (!util_getopt_short_from_long(long_options, optstring, sizeof(optstring))) {
		goto cleanup;
	}

	printf("%s\n", optstring);

	if (strcmp(optstring, "h1:2::4:")) {
		goto cleanup;
	}

	util_getopt_usage(stdout, argv[0], "bla bla", long_options);

	while (
		(opt = getopt_long(
			argc,
			argv,
			optstring,
			long_options,
			NULL
		)) != -1
	) {
		if (opt < 0x100) {
			printf("%c: %s\n", opt, optarg);
		}
		else {
			printf("%d: %s\n", opt, optarg);
		}
	}

	ret = 0;

cleanup:

	return ret;
}
