#include "splash_screen.h"
using namespace light_controller;

void SplashScreen::render() {
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  write_center("Settle", SCREEN_WIDTH / 2, 1, 2);
  write_center("Hackspace", SCREEN_WIDTH / 2, 17, 2);

  display.display();
}
