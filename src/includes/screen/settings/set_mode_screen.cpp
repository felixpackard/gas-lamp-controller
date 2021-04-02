#include "set_mode_screen.h"
using namespace light_controller;

void SetModeScreen::init() {
  timer = millis();
  AbstractScreen::init();
}

void SetModeScreen::update(bool menu_pressed, bool down_pressed, bool up_pressed) {
  const unsigned long m = millis();

  if (menu_pressed && cb_timeout) {
    cb_timeout();
  } else if (down_pressed || up_pressed) {
    timer = m;

    next_state();
    light_service.mode = state;
  }

  if (m - timer >= SCREEN_TIMEOUT) {
    if (cb_timeout) cb_timeout();
  }

  AbstractScreen::update(menu_pressed, down_pressed, up_pressed);
}

void SetModeScreen::render() {
  char buffer[7];

  switch (state) {
    case MODE_AUTO:
      strcpy_P(buffer, PSTR("AUTO"));
      break;
    case MODE_ON:
      strcpy_P(buffer, PSTR("ON"));
      break;
    case MODE_OFF:
      strcpy_P(buffer, PSTR("OFF"));
      break;
  }

  write_data(title, buffer);
  display.display();
}
