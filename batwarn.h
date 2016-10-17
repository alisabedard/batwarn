// batwarn - (C) 2015-2016 Jeffrey E. Bedard
#ifndef BATWARN_BATWARN_H
#define	BATWARN_BATWARN_H
#include <stdint.h>
enum {
	BW_BEEN_LOW = 1,
	BW_BATWARN_GAMMA_NORMAL = 2,
	BW_DEBUG = 4,
	BW_HIBERNATE = 8,
	BW_SUSPEND = 16
};
void batwarn_start_checking(uint8_t flags);
void batwarn_set_percent(const uint8_t pct);
#endif//!BATWARN_BATWARN_H
