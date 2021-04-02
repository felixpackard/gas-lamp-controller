#include "button.h"
using namespace light_controller;

bool Button::update() {
  btn->update();

  if (btn->fell()) {
    trigger_timer = millis();
    retrigger_delay = BUTTON_REPEAT_DELAY;
    return true;
  }
  
  if (btn->read() == LOW && millis() - trigger_timer >= retrigger_delay) {
    trigger_timer = millis();
    retrigger_delay = BUTTON_REPEAT_FREQ;
    return true;
  }
  
  return false;
}
