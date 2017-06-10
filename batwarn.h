// batwarn - (C) 2015-2017 Jeffrey E. Bedard
#ifndef BATWARN_BATWARN_H
#define	BATWARN_BATWARN_H
#include <stdint.h>
enum BatwarnFlags {
	BATWARN_BEEN_LOW = 1,
	BATWARN_BATWARN_GAMMA_NORMAL = (1 << 1),
	BATWARN_DEBUG = (1 << 2),
	BATWARN_HIBERNATE = (1 << 3),
	BATWARN_SUSPEND = (1 << 4),
	BATWARN_GAMMA_FAILED = (1 << 5)
};
void batwarn_start_checking(uint8_t flags);
void batwarn_set_percent(const uint8_t pct);
#endif//!BATWARN_BATWARN_H
