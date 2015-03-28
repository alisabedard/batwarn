/* 
 * File:   batwarn.h
 * Author: jefbed
 *
 * Created on March 27, 2015, 11:56 PM
 */

#ifndef BATWARN_H
#define	BATWARN_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define LOW_PERCENT 10
#define CRIT_PERCENT 5
#define WAIT 60
#define XGAMMA_CMD "xgamma -q -gamma"
#define GAMMA_NORMAL 1.25
#define GAMMA_WARNING 5.0
#define SYSFILE "/sys/class/power_supply/BAT0/capacity"
    
void
batwarn_set_gamma(const float value);

void
batwarn_start_checking();

#ifdef	__cplusplus
}
#endif

#endif	/* BATWARN_H */

