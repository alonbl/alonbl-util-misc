/**
 * @file
 * @brief getopt_long utilities.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <string.h>

#include "util-getopt.h"

void
util_getopt_usage(
	FILE *out,
	const char * const argv0,
	const char * const extra_usage,
	const struct option * const options
) {
	const struct option *option;

	fprintf(out, "usage: %s [options] %s\n", argv0, extra_usage);
	for (
		option = options;
		option->name != NULL;
		option++
	) {
		fprintf(out, "%2s", "");
		if (option->val < 0x100) {
			switch (option->has_arg) {
				default:
				case no_argument:
					fprintf(out, "-%c      ", option->val);
				break;
				case required_argument:
					fprintf(out, "-%c arg  ", option->val);
				break;
				case optional_argument:
					fprintf(out, "-%c [arg]", option->val);
				break;
			}
		}
		else {
			fprintf(out, "%8s", "");
		} 
		fprintf(out, " ");
		switch (option->has_arg) {
			default:
			case no_argument:
				fprintf(out, "--%s      ", option->name);
			break;
			case required_argument:
				fprintf(out, "--%s=arg  ", option->name);
			break;
			case optional_argument:
				fprintf(out, "--%s[=arg]", option->name);
			break;
		}
		fprintf(
			out,
			"\n%16s%s\n\n",
			"",
			option->name + strlen(option->name) + 1
		);
	}
}

int
util_getopt_short_from_long(
	const struct option * const options,
	char *const optstring,
	const size_t optstring_size
) {
	const struct option *option;
	char * p = optstring;

	for (
		option = options;
		(
			option->name != NULL &&
			(size_t)(p - optstring) < optstring_size - 4
		);
		option++
	) {
		if (option->val < 0x100) {
			*p++ = option->val;
			switch (option->has_arg) {
				case optional_argument:
					*p++ = ':';
					optstring[strlen(optstring)] = ':';
					/* NO BREAK */
					__attribute__ ((fallthrough));
				case required_argument:
					*p++ = ':';
					break;
			}
		}
	}

	*p = '\0';

	return option->name == NULL;
}
