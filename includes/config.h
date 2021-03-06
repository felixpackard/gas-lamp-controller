#ifndef CONFIG_H
#define CONFIG_H

namespace light_controller {

  #define MAX_SCREENS 4
  #define SCREEN_WIDTH 128
  #define SCREEN_HEIGHT 32

  #define IDLE_UPDATE_FREQ 4000
  #define SCREEN_TIMEOUT 10000

  #define BUTTON_PIN_1 8
  #define BUTTON_PIN_2 9
  #define BUTTON_PIN_3 10

  #define BUTTON_REPEAT_DELAY 750
  #define BUTTON_REPEAT_FREQ 100

  #define SERIAL_DEBUG true
  #define DEFAULT_TIME 1577836800000

}

#endif
