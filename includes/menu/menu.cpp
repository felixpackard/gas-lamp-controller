#include "menu.h"
using namespace ::light_controller;



// bool Menu::add_screen(Screen &screen) {
//   if (_screen_count < MAX_SCREENS) {
//     _screens[_screen_count] = &screen;
//     _screen_count++;
//     return true;
//   }

//   return false;
// }

// bool Menu::update(Adafruit_SSD1306 &display, Bounce &button_menu, Bounce &button_down, Bounce &button_up) {
//   Screen active_screen = *_screens[_active_screen];
//   bool action_handled = active_screen.update(button_menu, button_down, button_up);

//   if (action_handled) return false;

//   if (button_menu.fell()) {
//     _active_screen++;

//     if (_active_screen >= _screen_count) {
//       _active_screen = 0;
//     }

//     render(display);
//     return true;
//   }

//   if (button_down.fell()) {
//     render(display);
//   }

//   if (button_up.fell()) {
//     render(display);
//   }

//   return false;
// }

// void Menu::render(Adafruit_SSD1306 &display) {
//   Screen active_screen = *_screens[_active_screen];
//   active_screen.render(display);
// }

// bool Screen::update(Bounce &button_menu, Bounce &button_down, Bounce &button_up) {
//   if (button_menu.fell()) {
//     // no-op
//   }

//   if (button_down.fell()) {
//     // no-op
//   }

//   if (button_up.fell()) {
//     // no-op
//   }

//   return false;
// }

// void Screen::render(Adafruit_SSD1306 &display) {
//   write_data(display, _title, _data);
// }

// void Screen::write_data(Adafruit_SSD1306 &display, const char *title, const char *data) {
//   display.clearDisplay();
//   display.setTextColor(SSD1306_WHITE);

//   display.setTextSize(1);
//   display.setCursor(4, 4);
//   display.write(title);

//   display.setTextSize(2);
//   display.setCursor(4, 14);
//   display.write(data);

//   display.display();
// }
