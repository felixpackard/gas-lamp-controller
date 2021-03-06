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

    Serial.println("Idle next state");
  }

  time_t t = now();

  switch (state) {
    case CURRENT_TIME:
      set_title("Current time");
      format_time(data, t);
      break;
    case CURRENT_DATE:
      set_title("Current date");
      format_date(data, t);
      break;
  }

  AbstractScreen::update(menu_pressed, down_pressed, up_pressed);
}

void IdleScreen::render() {
  write_data(title, data);
}

void IdleScreen::set_title(const char *new_title) {
  strncpy(title, new_title, sizeof(title) - 1);
}
