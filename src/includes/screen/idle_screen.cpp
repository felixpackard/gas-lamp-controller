#include "idle_screen.h"
using namespace light_controller;

void IdleScreen::init() {
  timer = millis() + IDLE_UPDATE_FREQ;
  AbstractScreen::init();
}

void IdleScreen::update(bool menu_pressed, bool down_pressed, bool up_pressed) {
  const unsigned long m = millis();

  if (m > timer) {
    timer = m + IDLE_UPDATE_FREQ;
    next_state();
  }

  time_t t = now();

  switch (state) {
    case CURRENT_TIME:
      set_title(PSTR("Current time"));
      format_time(data, t);
      break;
    case CURRENT_DATE:
      set_title(PSTR("Current date"));
      format_date(data, t);
      break;
    case NEXT_ACTION:
      if (light_service.get_state() == ON) {
        set_title(PSTR("Turning off at"));
      } else {
        set_title(PSTR("Turning on at"));
      }

      time_t next_action_time = light_service.get_next_action_time();
      format_time(data, next_action_time);
      break;
  }

  AbstractScreen::update(menu_pressed, down_pressed, up_pressed);
}

void IdleScreen::render() {
  write_data(title, data);
  display.display();
}

void IdleScreen::set_title(const char *new_title) {
  strlcpy_P(title, new_title, strlen_P(title) + 1);
}
