#include "idle_screen.h"
using namespace light_controller;

void IdleScreen::update(bool menu_pressed, bool down_pressed, bool up_pressed) {
  if (menu_pressed) {
    state++;
    if (state > max_state) state = 0;
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

  render();
}

void IdleScreen::render() {
  write_data(title, data);
}

void IdleScreen::set_title(const char *new_title) {
  strncpy(title, new_title, sizeof(title) - 1);
}
