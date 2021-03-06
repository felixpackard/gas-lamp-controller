#include "settings_screen.h"
using namespace light_controller;

void SettingsScreen::update(bool menu_pressed, bool down_pressed, bool up_pressed) {
  if (menu_pressed) {
    state++;
    if (state > max_state) state = 0;
  }

  switch (state) {
    case SET_TIME:
      set_data("Set time");
      break;
    case SET_DATE:
      set_data("Set date");
      break;
    case SET_PWD:
      set_data("Set pwd");
      break;
  }

  AbstractScreen::update(menu_pressed, down_pressed, up_pressed);
}

void SettingsScreen::render() {
  write_data(title, data);
}

void SettingsScreen::set_data(const char *new_data) {
  strncpy(data, new_data, sizeof(data) - 1);
}
