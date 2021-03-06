#include "screen.h"
using namespace light_controller;

void AbstractScreen::update(bool menu_pressed, bool down_pressed, bool up_pressed) {
  render();
}

void AbstractScreen::write_center(const char *buf, int x, int y, int font_size) {
  display.setCursor(x - floor(strlen(buf) * ((font_size * 5) + 1) - 1) / 2, y);
  display.write(buf);
}

void AbstractScreen::write_data(const char *title, const char *data) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(4, 4);
  display.write(title);

  display.setTextSize(2);
  display.setCursor(4, 14);
  display.write(data);

  display.display();
}

ScreenState AbstractScreen::get_state() {
  return state;
}
