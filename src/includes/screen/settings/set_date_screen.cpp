#include "set_date_screen.h"
using namespace light_controller;

void SetDateScreen::init() {
  timer = millis();
  breakTime(now(), adjusted_date);
  AbstractScreen::init();
}

void SetDateScreen::update(bool menu_pressed, bool down_pressed, bool up_pressed) {
  const unsigned long m = millis();

  if (menu_pressed) {
    timer = m;
    state++;

    if (state > max_state) {
      tmElements_t current_time;
      breakTime(now(), current_time);

      adjusted_date.Hour = current_time.Hour;
      adjusted_date.Minute = current_time.Minute;
      adjusted_date.Second = current_time.Second;

      setTime(makeTime(adjusted_date));
      RTC.set(now());

      light_service.update_rise_set();
      if (cb_timeout) cb_timeout();

      return;
    }
  } else if (down_pressed || up_pressed) {
    timer = m;

    int8_t modifier = down_pressed ? -1 : 1;

    switch (state) {
      case SetDateScreenState::SET_YEAR:
        adjusted_date.Day = constrain(adjusted_date.Day, 1, Utils::count_days_in_month(adjusted_date.Month, max(adjusted_date.Year + modifier, 50)));
        adjusted_date.Year = max(adjusted_date.Year + modifier, 50);
        break;
      case SetDateScreenState::SET_MONTH:
        adjusted_date.Day = constrain(adjusted_date.Day, 1, Utils::count_days_in_month(constrain(adjusted_date.Month + modifier, 1, 12), adjusted_date.Year));
        adjusted_date.Month = constrain(adjusted_date.Month + modifier, 1, 12);
        break;
      case SetDateScreenState::SET_DAY:
        adjusted_date.Day = constrain(adjusted_date.Day + modifier, 1, Utils::count_days_in_month(adjusted_date.Month, adjusted_date.Year));
        break;
    }
  }

  if (m - timer >= SCREEN_TIMEOUT) {
    if (cb_timeout) cb_timeout();
  }

  AbstractScreen::update(menu_pressed, down_pressed, up_pressed);
}

void SetDateScreen::render() {
  char buffer[16];
  format_date_reverse(buffer, makeTime(adjusted_date));

  write_data(title, buffer);

  if (millis() / 500 % 2 == 0) {
    switch (state) {
      case SetDateScreenState::SET_YEAR:
        display.drawFastHLine(4, 31, 48, SSD1306_WHITE);
        break;
      case SetDateScreenState::SET_MONTH:
        display.drawFastHLine(64, 31, 22, SSD1306_WHITE);
        break;
      case SetDateScreenState::SET_DAY:
        display.drawFastHLine(100, 31, 22, SSD1306_WHITE);
        break;
    }
  }

  display.display();
}
