// batwarn - (C) 2015-2017 Jeffrey E. Bedard
#include "gamma.h"
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
// Returns true if successful.
void batwarn_set_gamma(const float gamma)
{
  bool r;
  Display * dpy = XOpenDisplay(NULL);
  if (dpy) {
    r = XF86VidModeSetGamma(dpy, DefaultScreen(dpy),
      &(XF86VidModeGamma){.red=gamma, .blue=gamma, .green=gamma});
    XCloseDisplay(dpy);
  } else {
    fputs("DISPLAY not available!", stderr);
    exit(1);
  }
  if (!r) {
    fputs("Cannot set gamma!", stderr);
    exit(1);
  }
}
