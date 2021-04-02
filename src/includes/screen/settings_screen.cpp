#include "settings_screen.h"
using namespace light_controller;

void SettingsScreen::init() {
  timer = millis();
  AbstractScreen::init();
}

void SettingsScreen::update(bool menu_pressed, bool down_pressed, bool up_pressed) {
  const unsigned long m = millis();

  if (menu_pressed) {
    timer = m;
    next_state();
  }

  if (cb_button_pressed) {
    cb_button_pressed(menu_pressed, down_pressed, up_pressed);
  }
  
  if (m - timer >= SCREEN_TIMEOUT) {
    if (cb_timeout) cb_timeout();
  }

  switch (state) {
    case SettingsScreenState::SET_TIME:
      set_string(data, PSTR("Set time"));
      break;
    case SettingsScreenState::SET_DATE:
      set_string(data, PSTR("Set date"));
      break;
    case SettingsScreenState::SET_MODE:
      set_string(data, PSTR("Set mode"));
      break;
  }

  AbstractScreen::update(menu_pressed, down_pressed, up_pressed);
}

void SettingsScreen::render() {
  write_data(title, data);
  display.display();
}
