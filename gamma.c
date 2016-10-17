// batwarn - (C) 2015-2016 Jeffrey E. Bedard
#include "gamma.h"
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>
void batwarn_set_gamma(const float gamma)
{
	Display * dpy = XOpenDisplay(NULL);
	XF86VidModeSetGamma(dpy, DefaultScreen(dpy),
		&(XF86VidModeGamma){.red=gamma,.blue=gamma,.green=gamma});
	XCloseDisplay(dpy);
}
