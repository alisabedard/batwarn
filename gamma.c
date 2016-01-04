// batwarn - (C) 2015-2016 Jeffrey E. Bedard

#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>

void
batwarn_set_gamma(const float gamma)
{
	Display *dpy = XOpenDisplay(NULL);
	XF86VidModeGamma g={.red=gamma,.blue=gamma,.green=gamma};
	XF86VidModeSetGamma(dpy, DefaultScreen(dpy), &g);
	XCloseDisplay(dpy);
}
