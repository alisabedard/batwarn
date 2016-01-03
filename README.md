# BATWARN
## About
batwarn is a simple X11 utility for notifying the user of low battery state
on laptops.  batwarn sets a high display gamma whenever the battery drops
below 10 percent.  The polling frequency increases at this point, in order
to quickly restore normal gamma when power is restored. 

## Requirements
batwarn requires the linux sysfs interface, the battery and ac ACPI modules,
and libXxf86vm.  Accessed interfaces and trip percentages can be set in
config.h.

Copyright 2015, Jeffrey E. Bedard

