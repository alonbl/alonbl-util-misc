#ifndef __UTIL_GETOPT_H
#define __UTIL_GETOPT_H

/**
 * @file
 * @brief getopt_long utilities.
 */

#include <stdio.h>

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#else
#define COMPAT_GETOPT_APPLY
#include "compat-getopt.h"
#endif

/**
 * Construct usage out of options.
 * @param out output file.
 * @param argv0 program file.
 * @param extra_usage some additional notes.
 * @param options the options, description is after null character.
 */
void
util_getopt_usage(
	FILE *out,
	const char * const argv0,
	const char * const extra_usage,
	const struct option * const options
);

/**
 * Construct short options out of long options.
 * @param options the options.
 * @param optstring the output short options.
 * @param optstring_size the output size.
 * @return 0 if buffer too small.
 */
int
util_getopt_short_from_long(
	const struct option * const options,
	char *const optstring,
	const size_t optstring_size
);

#endif
