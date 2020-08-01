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
static int8_t get_charge(void)
{
  /* Indicate good battery status when AC power is restored to restore
     gamma more quickly.  */
  return bw_get_value(BATWARN_SYS_AC_FILE) ? 100
  : bw_get_value(BATWARN_SYS_BATTERY_FILE);
}
static void handle_critical_battery(const uint8_t flags)
{
  if (flags & BATWARN_HIBERNATE)
    batwarn_execute(BATWARN_HIBERNATE_COMMAND);
  else if (flags & BATWARN_SUSPEND)
    batwarn_execute(BATWARN_SUSPEND_COMMAND);
}
static uint8_t handle_low_battery(uint8_t flags, const uint8_t charge)
{
  if (!(flags & BATWARN_BEEN_LOW)) {
    if (!batwarn_set_gamma(BATWARN_GAMMA_WARNING))
      flags |= BATWARN_GAMMA_FAILED;
    flags &= ~BATWARN_BATWARN_GAMMA_NORMAL;
    flags |= BATWARN_BEEN_LOW;
  }
  if (charge < BATWARN_PERCENT_CRITICAL)
    handle_critical_battery(flags);
  return flags;
}
static uint8_t handle_normal_battery(uint8_t flags)
{
  if (!(flags & BATWARN_BATWARN_GAMMA_NORMAL)) {
    if (!batwarn_set_gamma(BATWARN_GAMMA_NORMAL))
      flags |= BATWARN_GAMMA_FAILED;
    flags |= BATWARN_BATWARN_GAMMA_NORMAL;
    flags &= ~BATWARN_BEEN_LOW;
  }
  return flags;
}
uint8_t get_flags(const uint8_t charge, const uint8_t flags)
{
  return charge > low_percent ? handle_normal_battery(flags)
  : handle_low_battery(flags, charge);
}
static void warn_gamma_set_failed(void)
{
  char msg[] = "Cannot set gamma!\n";
  write(2, msg, sizeof msg);
}
void batwarn_start_checking(uint8_t flags)
{
  batwarn_execute("echo batwarn started at `date`");
  if (!low_percent)
    low_percent = BATWARN_PERCENT_LOW;
  LOG("low_percent: %d\n", low_percent);
  for (;;) {
    flags = get_flags(get_charge(), flags);
    if ((flags & BATWARN_GAMMA_FAILED))
      warn_gamma_set_failed();
    LOG("charge: %d\n", charge);
    enum { DELAY = 60 }; // Delay for checking system files
    // If low, check more frequently to restore gamma quickly:
    sleep((flags & BATWARN_BEEN_LOW) ? 1 : DELAY);
  }
}
