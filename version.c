// Copyright 2017, Jeffrey E. Bedard
#include "version.h"
#include <string.h>
#include "config.h"
static int get_places(const int n, const int value)
{
	return n > 0 ? get_places(n / 10, value + 1) : value;
}
static inline char get_char_for_int(const int n)
{
	return '0' + n % 10;
}
static void int_to_string(const int number, short place, char * string)
{
	if (place >= 0) {
		string[--place] = get_char_for_int(number);
		int_to_string(number / 10, place, string);
	}
}
static char * get_string(int n, short * restrict size)
{
	if (n == 0)
		return "0";
	enum {BUFSZ = 4};
	static char buf[BUFSZ];
	short p = *size = get_places(n, 0);
	int_to_string(n, p, buf);
	return buf;
}
static char * add_separator(char * restrict buf)
{
	*buf = '.';
	return buf + 1;
}
char * batwarn_get_version(int * restrict size_ret)
{
	/* A BUFSZ of 20 allows for a version number with three digits each
	 * for major, minor, and patch versions.  */
	enum {BUFSZ = 20};
	const char start[] = "Version ";
	static char buf[BUFSZ];
	char * p = buf, * q;
	short i = sizeof start;
	p = i + strncpy(p, start, i) - 1; // -1 to truncate terminator
	q = get_string(BATWARN_VERSION_MAJOR, &i);
	p = i + strncpy(p, q, i);
	p = add_separator(p);
	q = get_string(BATWARN_VERSION_MINOR, &i);
	p = i + strncpy(p, q, i);
	p = add_separator(p);
	q = get_string(BATWARN_VERSION_PATCH, &i);
	p = i + strncpy(p, q, i);
	p = 1 + strncpy(p, "\n", 1);
	*size_ret = p - buf;
	return buf;
}
