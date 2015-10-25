/* 
 * File:   config.h
 * Author: jefbed
 *
 * Created on March 29, 2015, 2:55 AM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C"
{
#endif

#define LOW_PERCENT 10
#define CRIT_PERCENT 5
#define FULL_PERCENT 90
#define WAIT 60
#define BATSYSFILE "/sys/class/power_supply/BAT0/capacity"
#define ACSYSFILE "/sys/class/power_supply/AC/online"
#define SUSPEND_CMD "systemctl suspend"
#define GAMMA_NORMAL    1.0
#define GAMMA_FULL      1.3
#define GAMMA_WARNING   5.0

#ifdef	__cplusplus
}
#endif

#endif				/* CONFIG_H */
