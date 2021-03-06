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

#include "includes/light_service/light_service.h"

#include "includes/screen/splash_screen.h"
#include "includes/screen/idle_screen.h"
#include "includes/screen/settings_screen.h"

namespace light_controller {

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
   * @brief Renders the currently active Screen
   * 
   */
  void render();

  /**
   * @brief Sets the current Screen to idle
   * 
   */
  void show_idle();

  /**
   * @brief Handles button presses whilst the idle Screen is shown
   * 
   */
  void idle_button_pressed(bool menu_pressed, bool down_pressed, bool up_pressed);

  /**
   * @brief Runs when the daily alarm is triggered
   * 
   */
  void daily_alarm_triggered();

}

#endif
