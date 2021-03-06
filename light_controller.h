#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

#include <Arduino.h>

// Time + sunrise & sunset
// #include <Time.h>
// #include <TimeAlarms.h>
#include <DS1307RTC.h>
// #include <Timezone.h>
// #include <Dusk2Dawn.h>

// Display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Buttons
#include <Bounce2.h>

// Utils
// #include <math.h>
#include <SPI.h>
#include <Wire.h>

// User includes
#include "includes/utils/utils.h"
#include "includes/button/button.h"

#include "includes/screen/splash_screen.h"
#include "includes/screen/idle_screen.h"
#include "includes/screen/settings_screen.h"

namespace light_controller {

  /**
   * @brief Refers to the current menu that's active.
   * 
   */
  enum LightControllerMenu {
    INIT,
    IDLE,
    SETTINGS
  };

  // #define BUTTON_REPEAT_DELAY 750
  // #define BUTTON_REPEAT_FREQ 100

  // #define LIGHT_PIN 6
  // #define MIN_BRIGHTNESS 100
  // #define MAX_BRIGHTNESS 255
  // #define RANDOM_AMOUNT 15
  // #define BRI_INTERVAL 25
  // #define FADE_SPEED 0.001

  // #define DEFAULT_TIME 1577865600

  // #define NUM_SCREENS 3
  // #define SCREEN_INTERVAL 4
  // #define BLANK_CYCLE 2

  // #define NUM_MENUS 2
  // #define MENU_TIMEOUT 10

  /**
   * @brief Renders the currently active Menu
   * 
   */
  void render();

  /**
   * @brief Sets the current Menu to idle
   * 
   */
  void show_idle();

}

#endif
