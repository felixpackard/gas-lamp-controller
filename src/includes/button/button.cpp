#include "button.h"
using namespace light_controller;

bool Button::update() {
  btn->update();

  if (btn->fell()) {
    next_trigger = millis() + BUTTON_REPEAT_DELAY;
    return true;
  }
  
  if (btn->read() == LOW && millis() > next_trigger) {
    next_trigger = millis() + BUTTON_REPEAT_FREQ;
    return true;
  }
  
  return false;
}
