// batwarn - (C) 2015-2016 Jeffrey E. Bedard

#ifndef BATWARN_H
#define	BATWARN_H

#include <stdint.h>

#define BW_BEEN_LOW		1
#define BW_GAMA_NORMAL		2
#define BW_DEBUG		4

void 
batwarn_start_checking(uint8_t flags);

#endif//BATWARN_H
