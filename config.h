// batwarn - (C) 2015-2017 Jeffrey E. Bedard
#ifndef BATWARN_CONFIG_H
#define	BATWARN_CONFIG_H
#define BATWARN_VERSION "20200804"
enum BatwarnSettings {
    ENABLE_HIBERNATE_BY_DEFAULT=1,
    ENABLE_SUSPEND_BY_DEFAULT=0
};
#define BATWARN_GAMMA_NORMAL    1.0
#define BATWARN_GAMMA_WARNING   5.0
enum BatwarnLevel {
    BATWARN_PERCENT_LOW=15,
    BATWARN_PERCENT_CRITICAL=(BATWARN_PERCENT_LOW>>1)
};
// System files to check:
#define BATWARN_SYS_BATTERY_FILE "/sys/class/power_supply/BAT0/capacity"
#define BATWARN_SYS_AC_FILE "/sys/class/power_supply/AC/online"
#ifndef USE_SYSTEMD
//#define BATWARN_SUSPEND_COMMAND "sudo pm-suspend"
// ensure the following command has suid bit set
#define BATWARN_SUSPEND_COMMAND "/usr/sbin/s2ram"
//#define BATWARN_HIBERNATE_COMMAND "sudo pm-hibernate"
// ensure the following command has suid bit set
#define BATWARN_HIBERNATE_COMMAND "/usr/sbin/s2disk"
#else//USE_SYSTEMD
#define BATWARN_SUSPEND_COMMAND "sudo systemctl suspend"
#define BATWARN_HIBERNATE_COMMAND "sudo systemctl hibernate"
#endif//!USE_SYSTEMD
#endif//!BATWARN_CONFIG_H
