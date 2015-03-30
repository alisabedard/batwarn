Copyright 2015, Jeffrey E. Bedard

# batwarn
Simple battery warning daemon using ibam and xgamma


This is a very lightweight daemon which notifies the user of low battery status 
by changing the X screen gamma.  Batwarn requires xgamma and sysfs or ibam.  
Batwarn occupies very little memory and is suitable for inclusion in one's
.xinitrc file.  