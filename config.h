// batwarn - (C) 2015-2016 Jeffrey E. Bedard

#ifndef BW_CONFIG_H
#define	BW_CONFIG_H

#define GAMMA_NORMAL    1.0
#define GAMMA_WARNING   5.0

enum PercentCat {
	CRIT_PERCENT=5,
	LOW_PERCENT=10,
	FULL_PERCENT=90
};

// Delay for checking system files:
#ifndef DEBUG
enum { WAIT = 60 };
#else//DEBUG
enum { WAIT = 1 };
#endif//!DEBUG

// System files to check:
#define BATSYSFILE "/sys/class/power_supply/BAT0/capacity"
#define ACSYSFILE "/sys/class/power_supply/AC/online"

#define SUSPEND_CMD "systemctl suspend"
#define HIBERNATE_CMD "systemctl hibernate"

#endif//!BW_CONFIG_H
