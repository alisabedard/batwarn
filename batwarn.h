// batwarn - (C) 2015-2017 Jeffrey E. Bedard
#ifndef BATWARN_BATWARN_H
#define	BATWARN_BATWARN_H
#include <stdint.h>
enum BatwarnFlags {
	BATWARN_ENABLE_DEBUG = 1,
	BATWARN_ENABLE_HIBERNATE = 2,
	BATWARN_ENABLE_SUSPEND = 4
};
void batwarn_start_checking(uint8_t flags);
void batwarn_set_percent(uint8_t const pct);
void batwarn_set_critical(uint8_t const pct);
#endif//!BATWARN_BATWARN_H
