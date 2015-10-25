#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>

void
batwarn_set_gamma(const float gamma)
{
	Display *dpy;
	XF86VidModeGamma g;

	dpy = XOpenDisplay(getenv("DISPLAY"));
	g.red = g.blue = g.green = gamma;

	XF86VidModeSetGamma(dpy, DefaultScreen(dpy), &g);
	XCloseDisplay(dpy);
}
