#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

#include <Arduino.h>

// Time + sunrise & sunset
#include <DS1307RTC.h>

// Display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Buttons
#include <Bounce2.h>

// Utils
#include <SPI.h>
#include <Wire.h>

// User includes
#include "includes/utils/utils.h"
#include "includes/button/button.h"

#include "includes/light_service/light_service.h"

#include "includes/screen/splash_screen.h"
#include "includes/screen/idle_screen.h"
#include "includes/screen/settings_screen.h"
#include "includes/screen/settings/set_time_screen.h"
#include "includes/screen/settings/set_date_screen.h"

namespace light_controller {

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
   * @brief Handles button presses whilst the settings Screen is shown
   * 
   */
  void settings_button_pressed(bool menu_pressed, bool down_pressed, bool up_pressed);

  /**
   * @brief Runs when the daily alarm is triggered
   * 
   */
  void daily_alarm_triggered();

}

#endif
