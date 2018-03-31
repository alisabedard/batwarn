// Copyright 2017, Jeffrey E. Bedard
#ifndef BATWARN_UTIL_H
#define BATWARN_UTIL_H
typedef char fd_t;
void batwarn_execute(const char * restrict cmd);
int bw_get_value(const char * fn);
void batwarn_print(const char * restrict msg);
#endif//!BATWARN_UTIL_H
