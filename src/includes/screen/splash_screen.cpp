#include "splash_screen.h"
using namespace light_controller;

void SplashScreen::render() {
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  char buf[10];

  strcpy_P(buf, PSTR("Settle"));
  write_center(buf, 1, 2);

  strcpy_P(buf, PSTR("Hackspace"));
  write_center(buf, 17, 2);

  display.display();
}
