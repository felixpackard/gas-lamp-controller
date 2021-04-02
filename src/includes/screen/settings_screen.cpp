#include "settings_screen.h"
using namespace light_controller;

void SettingsScreen::init() {
  timer = millis() + SCREEN_TIMEOUT;
  AbstractScreen::init();
}

void SettingsScreen::update(bool menu_pressed, bool down_pressed, bool up_pressed) {
  const unsigned long m = millis();

  if (menu_pressed) {
    timer = m + SCREEN_TIMEOUT;
    next_state();
  }

  if (cb_button_pressed) {
    cb_button_pressed(menu_pressed, down_pressed, up_pressed);
  }
  
  if (m > timer) {
    if (cb_timeout) cb_timeout();
  }

  switch (state) {
    case SET_TIME:
      set_string(data, PSTR("Set time"));
      break;
    case SET_DATE:
      set_string(data, PSTR("Set date"));
      break;
    case SET_PWD:
      set_string(data, PSTR("Set pwd"));
      break;
  }

  AbstractScreen::update(menu_pressed, down_pressed, up_pressed);
}

void SettingsScreen::render() {
  write_data(title, data);
  display.display();
}
