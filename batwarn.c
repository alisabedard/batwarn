// batwarn - (C) 2015-2018 Jeffrey E. Bedard
#include "batwarn.h"
#include <unistd.h> // for sleep()
#include "config.h"
#include "gamma.h"
#include "log.h"
#include "util.h"
static uint8_t critical_percent=BATWARN_PERCENT_CRITICAL,
               low_percent=BATWARN_PERCENT_LOW;
void batwarn_set_percent(uint8_t const pct)
{
  low_percent = pct;
}
void batwarn_set_critical(uint8_t const pct)
{
  critical_percent = pct;
}
static uint8_t get_charge(void)
{
  /* Indicate good battery status when AC power is restored to restore
     gamma more quickly.  */
  return batwarn_get_value(BATWARN_SYS_AC_FILE) ? 100 :
    batwarn_get_value(BATWARN_SYS_BATTERY_FILE);
}
static void perform_action_for_charge(uint8_t const charge,
  uint8_t const flags) {
  if (charge < BATWARN_PERCENT_CRITICAL) {
    if (flags & BATWARN_ENABLE_HIBERNATE)
      batwarn_execute(BATWARN_HIBERNATE_COMMAND);
    else if (flags & BATWARN_ENABLE_SUSPEND)
      batwarn_execute(BATWARN_SUSPEND_COMMAND);
  }
}
void batwarn_start_checking(uint8_t flags)
{
  //  batwarn_execute("echo batwarn started at `date`");
  if (!low_percent)
    low_percent = BATWARN_PERCENT_LOW;
  LOG("low_percent: %d\n", low_percent);
  for (;;) {
    uint8_t const charge = get_charge();
    batwarn_set_gamma(charge > BATWARN_PERCENT_FULL ? BATWARN_GAMMA_NORMAL :
      BATWARN_GAMMA_WARNING);
    perform_action_for_charge(charge, flags);
    sleep(3);
  }
}
