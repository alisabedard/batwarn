// Copyright 2017, Jeffrey E. Bedard
#ifndef BATWARN_UTIL_H
#define BATWARN_UTIL_H
void batwarn_quit(const char * restrict msg, const char * restrict arg);
void batwarn_execute(const char * restrict cmd);
int bw_get_value(const char * fn);
void batwarn_print(const char * restrict msg);
#endif//!BATWARN_UTIL_H
