// batwarn - (C) 2015-2017 Jeffrey E. Bedard
#ifndef BATWARN_BATWARN_H
#define	BATWARN_BATWARN_H
#include <stdint.h>
enum BatwarnFlags {
	BATWARN_BEEN_LOW = 1,
	BATWARN_BATWARN_GAMMA_NORMAL = 2,
	BATWARN_DEBUG = 4,
	BATWARN_HIBERNATE = 8,
	BATWARN_SUSPEND = 16
};
void batwarn_start_checking(uint8_t flags);
void batwarn_set_percent(const uint8_t pct);
#endif//!BATWARN_BATWARN_H
