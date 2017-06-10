// batwarn - (C) 2015-2017 Jeffrey E. Bedard
#include "gamma.h"
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>
#include <stdbool.h>
// Returns true if successful.
bool batwarn_set_gamma(const float gamma)
{
	Display * dpy = XOpenDisplay(NULL);
	const bool r = XF86VidModeSetGamma(dpy, DefaultScreen(dpy),
		&(XF86VidModeGamma){.red=gamma, .blue=gamma, .green=gamma});
	XCloseDisplay(dpy);
	return r;
}
