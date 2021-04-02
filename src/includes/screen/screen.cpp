#include "screen.h"
using namespace light_controller;

// int freeRam() {
//   extern int __heap_start, *__brkval; 
//   int v; 
//   return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
// }

void AbstractScreen::init() {
  state = 0;

  // Serial.println("AbstractScreen::init");
  // Serial.println(freeRam());
}

void AbstractScreen::update(bool menu_pressed, bool down_pressed, bool up_pressed) {
  if (cb_button_pressed) cb_button_pressed(menu_pressed, down_pressed, up_pressed);
  render();
}

void AbstractScreen::on_button_pressed(ButtonPressEvent cb) {
  cb_button_pressed = cb;
}

void AbstractScreen::on_timeout(TimeoutEvent cb) {
  cb_timeout = cb;
}

void AbstractScreen::write_center(const char *buf, uint8_t x, uint8_t y, uint8_t font_size) {
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
}

ScreenState AbstractScreen::get_state() {
  return state;
}

void AbstractScreen::next_state() {
  state++;
  if (state > max_state) state = 0;
}
