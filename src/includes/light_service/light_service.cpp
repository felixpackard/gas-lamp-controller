#include "light_service.h"
using namespace light_controller;

LightService &LightService::get_instance() {
  static LightService instance;
  return instance;
}

void LightService::update_rise_set() {
  time_t today = now();
  time_t tomorrow = today + 86400;
  bool dst_today = my_tz.locIsDST(today);
  bool dst_tomorrow = my_tz.locIsDST(tomorrow);

  // Update sunrise and sunset for today
  sunrise = rise_set_to_time(today, settle.sunrise(year(), month(), day(), dst_today));
  sunset = rise_set_to_time(today, settle.sunset(year(), month(), day(), dst_today));

  // Update sunrise and sunset for tomorrow
  next_sunrise = rise_set_to_time(tomorrow, settle.sunrise(year(tomorrow), month(tomorrow), day(tomorrow), dst_tomorrow));
  next_sunset = rise_set_to_time(tomorrow, settle.sunset(year(tomorrow), month(tomorrow), day(tomorrow), dst_tomorrow));
}

time_t LightService::rise_set_to_time(time_t t, int rise_set) {
  tmElements_t time_elements;
  breakTime(t, time_elements);

  time_elements.Second = 0;
  
  time_elements.Hour = floor(rise_set / 60);
  time_elements.Minute = floor(rise_set % 60);

  return makeTime(time_elements);
}

void LightService::update() {
  time_t t = now();
  unsigned long m = millis();

  if (t > sunset || t < sunrise) {
    state = STATE_ON;
    next_action_time = next_sunrise;
  } else {
    state = STATE_OFF;
    next_action_time = next_sunset;
  }

  if (bri_multiplier < 1 && ((state == STATE_ON && mode == MODE_AUTO) || mode == MODE_ON))
    bri_multiplier = constrain(bri_multiplier + FADE_SPEED, 0, 1);
  if (bri_multiplier > 0 && ((state == STATE_OFF && mode == MODE_AUTO) || mode == MODE_OFF))
    bri_multiplier = constrain(bri_multiplier - FADE_SPEED, 0, 1);
  
  if (m - bri_update_timer >= BRI_INTERVAL) {
    bri_update_timer = m;
    brightness += round(random(-RANDOM_AMOUNT, RANDOM_AMOUNT));
    brightness = constrain(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS);

    analogWrite(LIGHT_PIN, brightness * bri_multiplier);
  }
}

LightState LightService::get_state() {
  return state;
}

time_t LightService::get_next_action_time() {
  return next_action_time;
}
