#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void
batwarn_set_gamma(const float value)
{
	char * cmd = XGAMMA_CMD;
	const uint8_t buf_size = 14 + sizeof (cmd);
	char buf[buf_size];

	snprintf(buf, buf_size, "%s %f", cmd, value);
	system(buf);
}
