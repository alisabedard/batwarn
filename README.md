# BATWARN
## About
batwarn is a simple X11 utility for notifying the user of low battery state
on laptops.  batwarn sets a high display gamma whenever the battery drops
below 10 percent.  The polling frequency increases at this point, in order
to quickly restore normal gamma when power is restored. 

## Requirements
batwarn requires the sysfs interface, the battery and ac ACPI modules,
and libXxf86vm.  Accessed interfaces and trip percentages can be set in
config.h.  The battery status is polled using a relatively simple method of
text file access, so any system providing AC adapter and battery percentage in
a text file can be made to work with batwarn with some minor modifications.
If you port batwarn to a non-Linux system, please provide patches.

Copyright 2015-2017, Jeffrey E. Bedard
