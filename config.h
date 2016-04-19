// batwarn - (C) 2015-2016 Jeffrey E. Bedard

#ifndef CONFIG_H
#define	CONFIG_H

#define GAMMA_NORMAL    1.0
#define GAMMA_FULL      1.3
#define GAMMA_WARNING   5.0

#define LOW_PERCENT 10
#define FULL_PERCENT 90

#ifndef DEBUG
#define WAIT 60
#else//DEBUG
#define WAIT 1
#endif//!DEBUG

#define BATSYSFILE "/sys/class/power_supply/BAT0/capacity"
#define ACSYSFILE "/sys/class/power_supply/AC/online"

#define CRIT_PERCENT 5
#define SUSPEND_CMD "systemctl suspend"

#endif//CONFIG_H
