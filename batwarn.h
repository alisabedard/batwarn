// batwarn - (C) 2015-2016 Jeffrey E. Bedard

#ifndef BATWARN_H
#define	BATWARN_H

#include <stdint.h>

enum {
	BW_BEEN_LOW = 1,
	BW_GAMMA_NORMAL = 2,
	BW_DEBUG = 4
};

void batwarn_start_checking(uint8_t flags);

#endif//!BATWARN_H
