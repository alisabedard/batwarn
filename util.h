// Copyright 2017, Jeffrey E. Bedard
#ifndef BATWARN_UTIL_H
#define BATWARN_UTIL_H
typedef int fd_t;
void batwarn_execute(const char * restrict cmd);
int batwarn_get_value(const char * fn);
#endif//!BATWARN_UTIL_H
