#ifndef BUTTON_H
#define BUTTON_H

#include <Bounce2.h>

#include "../config.h"
#include "../utils/utils.h"

namespace light_controller {

  class Button {

  public:
    // Constructor
    Button(Bounce *bounce) : btn(bounce) { };

    bool update();
  
  private:
    Bounce *btn;
    unsigned long trigger_timer;
    unsigned int retrigger_delay;

  };

};

#endif
