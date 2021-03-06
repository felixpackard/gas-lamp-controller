#include "main.h"
using namespace light_controller;

Bounce bounce_menu = Bounce();
Bounce bounce_down = Bounce();
Bounce bounce_up = Bounce();

Button button_menu = Button(&bounce_menu);
Button button_down = Button(&bounce_down);
Button button_up = Button(&bounce_up);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, 4);

AlarmID_t splash_screen_timer;

LightService &light_service = LightService::get_instance();

AbstractScreen *current_screen = nullptr;

SplashScreen splash_screen(display);
IdleScreen idle_screen(display);
SettingsScreen settings_screen(display);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  /*
   * Initialize buttons
   */
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);

  bounce_menu.attach(BUTTON_PIN_1);
  bounce_down.attach(BUTTON_PIN_2);
  bounce_up.attach(BUTTON_PIN_3);

  bounce_menu.interval(5);
  bounce_down.interval(5);
  bounce_up.interval(5);

  /*
   * Initialize display
   */
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Utils::debug(LogLevel::ERROR, "Could not find display.");
    while (1);
  }

  /*
   * Initialize RTC
   */
  setSyncProvider(RTC.get);

  if (timeStatus() != timeSet) {
    Utils::debug(LogLevel::WARNING, "RTC time was not set. Loading default time.");

    RTC.set((unsigned long) DEFAULT_TIME);
    setTime((unsigned long) DEFAULT_TIME);
  } else {
    Utils::debug(LogLevel::INFO, "Loaded time from RTC.");
  }

  /*
   * Initialize LightService
   */
  light_service.update_rise_set();

  Alarm.alarmRepeat(0, 0, 5, light_controller::daily_alarm_triggered);

  /*
   * Initialize menus
   */
  idle_screen.on_button_pressed(light_controller::idle_button_pressed);
  settings_screen.on_timeout(light_controller::show_idle);

  current_screen = &splash_screen;
  Utils::set_timer(splash_screen_timer, 2, light_controller::show_idle);
}

void loop() {
  /*
   * Update light service
   */
  light_service.update();

  /*
   * Update and render current menu
   */
  if (current_screen) current_screen->update(
    button_menu.update(),
    button_down.update(),
    button_up.update()
  );

  Alarm.delay(0);
}

void light_controller::show_idle() {
  idle_screen.init();
  current_screen = &idle_screen;
}

void light_controller::idle_button_pressed(bool menu_pressed, bool down_pressed, bool up_pressed) {
  if (menu_pressed) {
    settings_screen.init();
    current_screen = &settings_screen;
  }
}

void light_controller::daily_alarm_triggered() {
  light_service.update_rise_set();
}

// void light_controller::show_idle() {
//   current_menu = &menu_idle;
//   current_menu->render(display);
// }

// TimeChangeRule gmtDST = {"GMT", Last, Sun, Mar, 1, 60};
// TimeChangeRule gmtSTD = {"GMT", Last, Sun, Oct, 2, 0};
// Timezone myTZ(gmtDST, gmtSTD);

// TimeChangeRule *tcr;

// Dusk2Dawn settle(54.068241, -2.277658, 0);

// int bri = 0;
// int blankCounter = 0;

// float briMultiplier = 0;

// unsigned long lastBriUpdate = -1;
// unsigned long btnMenuNextTrigger = -1;
// unsigned long btnUpNextTrigger = -1;
// unsigned long btnDownNextTrigger = -1;

// char* dateBuf = (char*)malloc(9);
// char* timeBuf = (char*)malloc(9);
// char* timeIntBuf = (char*)malloc(9);
// char* dateTimeBuf = (char*)malloc(20);

// time_t sunrise;
// time_t sunset;
// time_t nextSunrise;
// time_t nextSunset;

// AlarmID_t idleTimer;
// AlarmID_t menuTimer;

// tmElements_t adjustedTime;
// tmElements_t adjustedDate;

// // State management
// enum DisplayStates { DISPLAY_IDLE, DISPLAY_MENU };
// enum IdleStates { WAITING, SPLASH, CURRENT_TIME, CURRENT_DATE, NEXT_ACTION };
// enum MenuStates { SETTINGS_SET_TIME, SETTINGS_SET_DATE, SET_TIME, SET_DATE };
// enum SetTimeStates { SET_HOUR, SET_MINUTE };
// enum SetDateStates { SET_DAY, SET_MONTH, SET_YEAR };
// enum LightStates { ON, OFF };

// DisplayStates displayState = DISPLAY_IDLE;
// IdleStates idleState = SPLASH;
// MenuStates menuState = SETTINGS_SET_TIME;
// SetTimeStates setTimeState;
// SetDateStates setDateState;
// LightStates lightState;

// IdleStates nextIdleState;

// void menuUseEvent();
// void menuMoveEvent();

// void updateState();
// void updateIdleState();
// void changeIdleState();

// bool updateButtonRepeat(Bounce *btn, unsigned long *nextTriggerPtr);

// void updateMenuState();
// void closeMenu();

// void updateIdleTimer(int seconds);
// void cancelIdleTimer();
// void updateMenuTimer(int seconds);
// void cancelMenuTimer();

// void drawSplashScreen();
// void writeCenter(const char *title, const char *data);
// void writeData(const char *title, const char *data);

// void updateRiseSet();

// void setup () {
//   Serial.begin(9600);

//   // Wait until the serial monitor opens
//   while (!Serial);

//   // Sync system time with the rtc
//   setSyncProvider(RTC.get);

//   // Make sure rtc is set
//   if (timeStatus() != timeSet) {
//     Serial.println("[WARN] RTC time was not set.");

//     // Update rtc and system time
//     RTC.set(DEFAULT_TIME);
//     setTime(DEFAULT_TIME);
//   } else {
//     Serial.println("[INFO] Loaded time from RTC.");
//   }

//   // Initialise the display
//   if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
//     Serial.println("[ERR] Could not find display.");
//     while (1);
//   }

//   updateRiseSet();

//   // Update sunrise and sunset times every day 5 seconds past midnight
//   Alarm.alarmRepeat(0, 0, 5, updateRiseSet);

//   // Set up buttons
//   pinMode(8, INPUT_PULLUP);
//   pinMode(9, INPUT_PULLUP);
//   pinMode(10, INPUT_PULLUP);

//   buttonMenu.attach(BUTTON_PIN_1);
//   buttonUp.attach(BUTTON_PIN_2);
//   buttonDown.attach(BUTTON_PIN_3);

//   buttonMenu.interval(5);
//   buttonUp.interval(5);
//   buttonDown.interval(5);
  
//   // Set up light output
//   pinMode(LIGHT_PIN, OUTPUT);
//   randomSeed(analogRead(0));

//   // Draw the splash screen
//   drawSplashScreen();
// }

// void loop () {
//   // unsigned long m = millis();
  
//   // buttonMenu.update();
//   // buttonUp.update();
//   // buttonDown.update();

//   // updateState();

//   // // Fade light up or down if necessary
//   // if (briMultiplier < 1 && lightState == ON)  briMultiplier = constrain(briMultiplier + FADE_SPEED, 0, 1);
//   // if (briMultiplier > 0 && lightState == OFF) briMultiplier = constrain(briMultiplier - FADE_SPEED, 0, 1);
  
//   // if (m > lastBriUpdate + BRI_INTERVAL) {
//   //   lastBriUpdate = m;
//   //   bri += round(random(-RANDOM_AMOUNT, RANDOM_AMOUNT));
//   //   bri = constrain(bri, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
//   //   analogWrite(LIGHT_PIN, bri * briMultiplier);
//   // }
  
//   // Alarm.delay(0);
// }

// void updateState() {
//   // Update light state
//   time_t t = now();
//   if (t > sunset || t < sunrise) {
//     lightState = ON;
//   } else {
//     lightState = OFF;
//   }
  
//   // Update display state
//   switch(displayState) {
//     case DISPLAY_IDLE:
//       updateIdleState();
//       break;
//     case DISPLAY_MENU:
//       updateMenuState();
//       break;
//   }
// }

// void updateIdleState() {
//   time_t t = now();

//   bool buttonMenuFell = updateButtonRepeat(&buttonMenu, &btnMenuNextTrigger);

//   if (buttonMenuFell) {
//     displayState = DISPLAY_MENU;
//     menuState = SETTINGS_SET_TIME;
//     cancelIdleTimer();
//     updateMenuTimer(MENU_TIMEOUT);
//     return;
//   }
  
//   switch(idleState) {
//     case SPLASH:
//       drawSplashScreen();
//       nextIdleState = CURRENT_TIME;
//       updateIdleTimer(2);
//       idleState = WAITING;
//       break;
//     case CURRENT_TIME:
//       Utils::formatTime(timeBuf, t);
//       writeData("CURRENT TIME", timeBuf);

//       display.display();
//       nextIdleState = CURRENT_DATE;
//       updateIdleTimer(SCREEN_INTERVAL);
//       idleState = WAITING;
//       break;
//     case CURRENT_DATE:
//       Utils::formatDate(dateBuf, t);
//       writeData("CURRENT DATE", dateBuf);
//       display.display();
//       nextIdleState = NEXT_ACTION;
//       updateIdleTimer(SCREEN_INTERVAL);
//       idleState = WAITING;
//       break;
//     case NEXT_ACTION:
//       if (lightState == ON) {
//         time_t data = sunrise;
//         if (t > sunrise) data = nextSunrise;

//         Utils::formatTime(timeBuf, data);
//         writeData("TURNING OFF AT", timeBuf);
//         display.display();
//       } else {
//         time_t data = sunset;
//         if (t > sunset) data = nextSunset;

//         Utils::formatTime(timeBuf, data);
//         writeData("TURNING ON AT", timeBuf);
//         display.display();
//       }
//       nextIdleState = CURRENT_TIME;
//       updateIdleTimer(SCREEN_INTERVAL);
//       idleState = WAITING;
//       break;
//   }
// }

// void changeIdleState() {
//   idleState = nextIdleState;
// }

// bool updateButtonRepeat(Bounce *btn, unsigned long *nextTriggerPtr) {
//   if (btn->fell()) {
//     *nextTriggerPtr = millis() + BUTTON_REPEAT_DELAY;
//     return true;
//   }
  
//   if (btn->read() == LOW && millis() > *nextTriggerPtr) {
//     *nextTriggerPtr = millis() + BUTTON_REPEAT_FREQ;
//     return true;
//   }
  
//   return false;
// }

// void updateMenuState() {
//   time_t t = now();
//   unsigned long m = millis();

//   bool buttonMenuFell = updateButtonRepeat(&buttonMenu, &btnMenuNextTrigger);
//   bool buttonUpFell = updateButtonRepeat(&buttonUp, &btnUpNextTrigger);
//   bool buttonDownFell = updateButtonRepeat(&buttonDown, &btnDownNextTrigger);
  
//   if (buttonMenuFell || buttonUpFell || buttonDownFell) {
//     // Refresh the menu timeout when a button is pressed
//     updateMenuTimer(MENU_TIMEOUT);
//   }

//   // Read inputs and update state
//   if (buttonMenuFell) {
//     switch (menuState) {
//       case SETTINGS_SET_TIME:
//         menuState = SETTINGS_SET_DATE;
//         break;
//       case SETTINGS_SET_DATE:
//         menuState = SETTINGS_SET_TIME;
//         break;
//       case SET_TIME:
//         switch (setTimeState) {
//           case SET_HOUR:
//             setTimeState = SET_MINUTE;
//             break;
//           case SET_MINUTE:
//             displayState = DISPLAY_IDLE;
//             idleState = CURRENT_TIME;
//             setTime(makeTime(adjustedTime));
//             RTC.set(now());
//             updateRiseSet();
//             break;
//         }
//         break;
//       case SET_DATE:
//         switch (setDateState) {
//           case SET_YEAR:
//             setDateState = SET_MONTH;
//             updateRiseSet();
//             break;
//           case SET_MONTH:
//             setDateState = SET_DAY;
//             break;
//           case SET_DAY:
//             displayState = DISPLAY_IDLE;
//             idleState = CURRENT_TIME;
//             adjustedDate.Hour = hour();
//             adjustedDate.Minute = minute();
//             adjustedDate.Second = second();
//             setTime(makeTime(adjustedDate));
//             RTC.set(now());
//             updateRiseSet();
//             break;
//         }
//         break;
//     }
//   }

//   if (buttonUpFell || buttonDownFell) {
//     int modifier = 1;
//     if (buttonDownFell) modifier = -1;
    
//     switch(menuState) {
//       case SETTINGS_SET_TIME:
//         menuState = SET_TIME;
//         setTimeState = SET_HOUR;
//         breakTime(now(), adjustedTime);
//         adjustedTime.Second = 0;
//         break;
//       case SETTINGS_SET_DATE:
//         menuState = SET_DATE;
//         setDateState = SET_YEAR;
//         breakTime(now(), adjustedDate);
//         break;
//       case SET_TIME:
//         switch (setTimeState) {
//           case SET_HOUR:
//             adjustedTime.Hour = constrain(adjustedTime.Hour + modifier, 0, 23);
//             break;
//           case SET_MINUTE:
//             adjustedTime.Minute = constrain(adjustedTime.Minute + modifier, 0, 59);
//             break;
//         }
//         break;
//       case SET_DATE:
//         switch (setDateState) {
//           case SET_YEAR:
//             adjustedDate.Day = constrain(adjustedDate.Day, 1, Utils::countDaysInMonth(adjustedDate.Month, max(adjustedDate.Year + modifier, 50)));
//             adjustedDate.Year = max(adjustedDate.Year + modifier, 50);
//             break;
//           case SET_MONTH:
//             adjustedDate.Day = constrain(adjustedDate.Day, 1, Utils::countDaysInMonth(constrain(adjustedDate.Month + modifier, 1, 12), adjustedDate.Year));
//             adjustedDate.Month = constrain(adjustedDate.Month + modifier, 1, 12);
//             break;
//           case SET_DAY:
//             adjustedDate.Day = constrain(adjustedDate.Day + modifier, 1, Utils::countDaysInMonth(adjustedDate.Month, adjustedDate.Year));
//             break;
//         }
//     }
//   }

//   // Render current state
//   switch (menuState) {
//     case SETTINGS_SET_TIME:
//       writeData("SETTINGS", "Set time");
//       display.display();
//       break;
//     case SETTINGS_SET_DATE:
//       writeData("SETTINGS", "Set date");
//       display.display();
//       break;
//     case SET_TIME:
//       Utils::formatTime(timeBuf, makeTime(adjustedTime));
//       writeData("SET TIME", timeBuf);

//       switch (setTimeState) {
//         case SET_HOUR:
//           if (millis() / 500 % 2 == 0)
//             display.drawFastHLine(4, 31, 22, SSD1306_WHITE);
//           break;
//         case SET_MINUTE:
//           if (millis() / 500 % 2 == 0)
//             display.drawFastHLine(40, 31, 22, SSD1306_WHITE);
//           break;
//       }

//       display.display();
//       break;
//     case SET_DATE:
//       Utils::formatDateReverse(dateBuf, makeTime(adjustedDate));
//       writeData("SET DATE", dateBuf);

//       switch (setDateState) {
//         case SET_YEAR:
//           if (millis() / 500 % 2 == 0)
//             display.drawFastHLine(4, 31, 48, SSD1306_WHITE);
//           break;
//         case SET_MONTH:
//           if (millis() / 500 % 2 == 0)
//             display.drawFastHLine(64, 31, 22, SSD1306_WHITE);
//           break;
//         case SET_DAY:
//           if (millis() / 500 % 2 == 0)
//             display.drawFastHLine(100, 31, 22, SSD1306_WHITE);
//           break;
//       }

//       display.display();
//       break;
//   }
// }

// void closeMenu() {
//   displayState = DISPLAY_IDLE;
//   idleState = CURRENT_TIME;
// }

// void updateIdleTimer(int seconds) {
//   // Cancel the previous timer if it exists
//   Alarm.disable(idleTimer);
//   Alarm.free(idleTimer);

//   // Set a new timer for the number of seconds specified and return the id
//   idleTimer = Alarm.timerOnce(seconds, changeIdleState);
// }

// void cancelIdleTimer() {
//   // Cancel the timer if it exists
//   Alarm.disable(idleTimer);
//   Alarm.free(idleTimer);
// }

// void updateMenuTimer(int seconds) {
//   // Cancel the previous timer if it exists
//   Alarm.disable(menuTimer);
//   Alarm.free(menuTimer);

//   // Set a new timer for the number of seconds specified and return the id
//   menuTimer = Alarm.timerOnce(seconds, closeMenu);
// }

// void cancelMenuTimer() {
//   // Cancel the timer if it exists
//   Alarm.disable(menuTimer);
//   Alarm.free(menuTimer);
// }

// void drawSplashScreen() {
//   display.clearDisplay();

//   display.setTextSize(2);
//   display.setTextColor(SSD1306_WHITE);

//   writeCenter("Settle", SCREEN_WIDTH / 2, 1, 2);
//   writeCenter("Hackspace", SCREEN_WIDTH / 2, 17, 2);

//   display.display();
// }

// void writeCenter(const char *buf, int x, int y, int fontSize)
// {
//   display.setCursor(x - floor(strlen(buf) * ((fontSize * 5) + 1) - 1) / 2, y);
//   display.write(buf);
// }


// void writeData(const char *title, const char *data) {
//   display.clearDisplay();
//   display.setTextColor(SSD1306_WHITE);

//   display.setTextSize(1);
//   display.setCursor(4, 4);
//   display.write(title);

//   display.setTextSize(2);
//   display.setCursor(4, 14);
//   display.write(data);
// }

// void updateRiseSet() {
//   time_t today = now();
//   time_t tomorrow = today + 86400;
//   bool dstToday = myTZ.locIsDST(today);
//   bool dstTomorrow = myTZ.locIsDST(tomorrow);

//   // Update sunrise and sunset for today
//   sunrise = Utils::riseSetToTime(today, settle.sunrise(year(), month(), day(), dstToday));
//   sunset = Utils::riseSetToTime(today, settle.sunset(year(), month(), day(), dstToday));

//   // Update sunrise and sunset for tomorrow
//   nextSunrise = Utils::riseSetToTime(tomorrow, settle.sunrise(year(tomorrow), month(tomorrow), day(tomorrow), dstTomorrow));
//   nextSunset = Utils::riseSetToTime(tomorrow, settle.sunset(year(tomorrow), month(tomorrow), day(tomorrow), dstTomorrow));
// }
