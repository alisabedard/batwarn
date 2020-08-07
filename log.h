// Copyright 2017, Jeffrey E. Bedard
#ifndef BATWARN_LOG_H
#define BATWARN_LOG_H
#ifdef LOG
#undef LOG
#endif//LOG
#ifdef DEBUG
#include <stdio.h>
#define LOG(...) fprintf(stderr, __VA_ARGS__)
#else//!DEBUG
#define LOG(...)
#endif//DEBUG
#endif//!BATWARN_LOG_H
