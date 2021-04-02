#include "set_time_screen.h"
using namespace light_controller;

void SetTimeScreen::init() {
  timer = millis();
  breakTime(now(), adjusted_time);
  AbstractScreen::init();
}

void SetTimeScreen::update(bool menu_pressed, bool down_pressed, bool up_pressed) {
  const unsigned long m = millis();

  if (menu_pressed) {
    timer = m;
    state++;

    if (state > max_state) {
      setTime(makeTime(adjusted_time));
      RTC.set(now());

      light_service.update_rise_set();
      if (cb_timeout) cb_timeout();

      return;
    }
  } else if (down_pressed || up_pressed) {
    timer = m;

    int modifier = 1;
    if (down_pressed) modifier = -1;

    switch (state) {
      case SET_HOUR:
        adjusted_time.Hour = constrain(adjusted_time.Hour + modifier, 0, 23);
        break;
      case SET_MINUTE:
        adjusted_time.Minute = constrain(adjusted_time.Minute + modifier, 0, 59);
        break;
    }
  }

  if (m - timer >= SCREEN_TIMEOUT) {
    if (cb_timeout) cb_timeout();
  }

  switch (state) {
    case SET_HOUR:
      set_string(data, "Set hour");
      break;
    case SET_MINUTE:
      set_string(data, "Set minute");
      break;
  }

  AbstractScreen::update(menu_pressed, down_pressed, up_pressed);
}

void SetTimeScreen::render() {
  char buffer[16];
  format_time(buffer, makeTime(adjusted_time));

  write_data(title, buffer);

  if (millis() / 500 % 2 == 0) {
    display.drawFastHLine(state == SET_HOUR ? 4 : 40, 31, 22, SSD1306_WHITE);
  }

  display.display();
}
