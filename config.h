// batwarn - (C) 2015-2016 Jeffrey E. Bedard

#ifndef CONFIG_H
#define	CONFIG_H

#define GAMMA_NORMAL    1.0
#define GAMMA_WARNING   5.0

enum {
	CRIT_PERCENT=5
	,LOW_PERCENT=10
	,FULL_PERCENT=90
};

#ifndef DEBUG
#define WAIT 60
#else//DEBUG
#define WAIT 1
#endif//!DEBUG

#define BATSYSFILE "/sys/class/power_supply/BAT0/capacity"
#define ACSYSFILE "/sys/class/power_supply/AC/online"

#define SUSPEND_CMD "systemctl suspend"

#endif//CONFIG_H
