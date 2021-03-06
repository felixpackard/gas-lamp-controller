#include "light_service.h"
using namespace light_controller;

LightService &LightService::get_instance() {
  static LightService instance;
  return instance;
}

void LightService::update_rise_set() {
  time_t today = now();
  time_t tomorrow = today + 86400;
  bool dstToday = my_tz.locIsDST(today);
  bool dstTomorrow = my_tz.locIsDST(tomorrow);

  // Update sunrise and sunset for today
  sunrise = rise_set_to_time(today, settle.sunrise(year(), month(), day(), dstToday));
  sunset = rise_set_to_time(today, settle.sunset(year(), month(), day(), dstToday));

  // Update sunrise and sunset for tomorrow
  next_sunrise = rise_set_to_time(tomorrow, settle.sunrise(year(tomorrow), month(tomorrow), day(tomorrow), dstTomorrow));
  next_sunset = rise_set_to_time(tomorrow, settle.sunset(year(tomorrow), month(tomorrow), day(tomorrow), dstTomorrow));
}

time_t LightService::rise_set_to_time(time_t t, int riseSet) {
  tmElements_t timeElements;
  breakTime(t, timeElements);

  timeElements.Second = 0;
  
  timeElements.Hour = floor(riseSet / 60);
  timeElements.Minute = floor(riseSet % 60);

  return makeTime(timeElements);
}

void LightService::update() {
  time_t t = now();

  if (t > sunset || t < sunrise) {
    state = ON;
    next_action_time = next_sunrise;
  } else {
    state = OFF;
    next_action_time = next_sunset;
  }
}

LightState LightService::get_state() {
  return state;
}

time_t LightService::get_next_action_time() {
  return next_action_time;
}
