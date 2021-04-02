#include "utils.h"
using namespace light_controller;

AlarmID_t Utils::set_timer(AlarmID_t timer, int seconds, TimerEvent cb) {
  Alarm.disable(timer);
  Alarm.free(timer);

  return Alarm.timerOnce(seconds, cb);
}

uint8_t Utils::count_days_in_month(uint8_t month, uint8_t year) {
  uint8_t days = days_per_month[month - 1];
  if (month - 1 == 1 && leap_year(year)) days++;
  return days;
}
