// batwarn - (C) 2015-2016 Jeffrey E. Bedard

#ifndef BATWARN_CONFIG_H
#define	BATWARN_CONFIG_H

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
#define BATWARN_SYS_BATTERY_FILE "/sys/class/power_supply/BAT0/capacity"
#define BATWARN_SYS_AC_FILE "/sys/class/power_supply/AC/online"

#define BATWARN_SUSPEND_COMMAND "systemctl suspend"
#define BATWARN_HIBERNATE_COMMAND "systemctl hibernate"

#endif//!BATWARN_CONFIG_H
