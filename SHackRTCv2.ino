// time + sunrise & sunset
#include <Time.h>
#include <TimeAlarms.h>
#include <DS1307RTC.h>
#include <Timezone.h>
#include <Dusk2Dawn.h>

// display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// buttons
#include <Bounce2.h>

// utils
#include <math.h>
#include <SPI.h>
#include <Wire.h>

#define BUTTON_PIN_1 8
#define BUTTON_PIN_2 9
#define BUTTON_PIN_3 10
#define BUTTON_REPEAT_DELAY 750
#define BUTTON_REPEAT_FREQ 100

#define LIGHT_PIN 6
#define MIN_BRIGHTNESS 100
#define MAX_BRIGHTNESS 255
#define RANDOM_AMOUNT 15
#define BRI_INTERVAL 25
#define FADE_SPEED 0.001

#define DEFAULT_TIME 1577865600

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define NUM_SCREENS 3
#define SCREEN_INTERVAL 4
#define BLANK_CYCLE 2

#define NUM_MENUS 2
#define MENU_TIMEOUT 10

#define LEAP_YEAR(Y)     ( ((1970+(Y))>0) && !((1970+(Y))%4) && ( ((1970+(Y))%100) || !((1970+(Y))%400) ) )

Bounce buttonMenu = Bounce();
Bounce buttonUp = Bounce();
Bounce buttonDown = Bounce();

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, 4);

TimeChangeRule gmtDST = {"GMT", Last, Sun, Mar, 1, 60};
TimeChangeRule gmtSTD = {"GMT", Last, Sun, Oct, 2, 0};
Timezone myTZ(gmtDST, gmtSTD);

TimeChangeRule *tcr;

Dusk2Dawn settle(54.068241, -2.277658, 0);

int daysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int bri = 0;
int blankCounter = 0;

float briMultiplier = 0;

unsigned long lastBriUpdate = -1;
unsigned long btnMenuNextTrigger = -1;
unsigned long btnUpNextTrigger = -1;
unsigned long btnDownNextTrigger = -1;

char* dateBuf = (char*)malloc(9);
char* timeBuf = (char*)malloc(9);
char* timeIntBuf = (char*)malloc(9);
char* dateTimeBuf = (char*)malloc(20);
char* logBuf = (char*)malloc(100);

time_t sunrise;
time_t sunset;
time_t nextSunrise;
time_t nextSunset;

AlarmID_t idleTimer;
AlarmID_t menuTimer;

tmElements_t adjustedTime;
tmElements_t adjustedDate;

// state management
enum DisplayStates { DISPLAY_IDLE, DISPLAY_MENU };
enum IdleStates { WAITING, SPLASH, CURRENT_TIME, CURRENT_DATE, NEXT_ACTION };
enum MenuStates { SETTINGS_SET_TIME, SETTINGS_SET_DATE, SET_TIME, SET_DATE };
enum SetTimeStates { SET_HOUR, SET_MINUTE };
enum SetDateStates { SET_DAY, SET_MONTH, SET_YEAR };
enum LightStates { ON, OFF };

DisplayStates displayState = DISPLAY_IDLE;
IdleStates idleState = SPLASH;
MenuStates menuState = SETTINGS_SET_TIME;
SetTimeStates setTimeState;
SetDateStates setDateState;
LightStates lightState;

IdleStates nextIdleState;

void setup () {
  Serial.begin(9600);
  while (!Serial); // wait until the serial monitor opens

  setSyncProvider(RTC.get); // sync system time with the rtc

  if (timeStatus() != timeSet) { // make sure rtc is set
    Serial.println("Warn: RTC time was not set.");
    RTC.set(DEFAULT_TIME); // update rtc and system time
    setTime(DEFAULT_TIME);
  } else {
    Serial.println("Info: Loaded time from RTC.");
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //init display
    Serial.println("Err: Could not find display.");
    while (1); // loop forever
  }

  updateRiseSet();
  Alarm.alarmRepeat(0, 0, 5, updateRiseSet); // update sunrise and sunset times every day 5 seconds past midnight

  // set up buttons
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);

  buttonMenu.attach(BUTTON_PIN_1);
  buttonUp.attach(BUTTON_PIN_2);
  buttonDown.attach(BUTTON_PIN_3);

  buttonMenu.interval(5);
  buttonUp.interval(5);
  buttonDown.interval(5);
  
  pinMode(6, OUTPUT);
  randomSeed(analogRead(0));

  drawSplashScreen();
}

void loop () {
  unsigned long m = millis();
  
  buttonMenu.update();
  buttonUp.update();
  buttonDown.update();

  updateState();

  // fade light up or down if necessary
  if (briMultiplier < 1 && lightState == ON)  briMultiplier = constrain(briMultiplier + FADE_SPEED, 0, 1);
  if (briMultiplier > 0 && lightState == OFF) briMultiplier = constrain(briMultiplier - FADE_SPEED, 0, 1);
  
  if (m > lastBriUpdate + BRI_INTERVAL) {
    lastBriUpdate = m;
    bri += round(random(-RANDOM_AMOUNT, RANDOM_AMOUNT));
    bri = constrain(bri, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    analogWrite(LIGHT_PIN, bri * briMultiplier);
  }
  
  Alarm.delay(0);
}

void updateState() {
  // update light state
  time_t t = now();
  if (t > sunset || t < sunrise) {
    lightState = ON;
  } else {
    lightState = OFF;
  }
  
  // update display state
  switch(displayState) {
    case DISPLAY_IDLE:
      updateIdleState();
      break;
    case DISPLAY_MENU:
      updateMenuState();
      break;
  }
}

void updateIdleState() {
  time_t t = now();

  bool buttonMenuFell = updateButtonRepeat(&buttonMenu, &btnMenuNextTrigger);

  if (buttonMenuFell) {
    displayState = DISPLAY_MENU;
    menuState = SETTINGS_SET_TIME;
    cancelIdleTimer();
    updateMenuTimer(MENU_TIMEOUT);
    return;
  }
  
  switch(idleState) {
    case SPLASH:
      drawSplashScreen();
      nextIdleState = CURRENT_TIME;
      updateIdleTimer(2);
      idleState = WAITING;
      break;
    case CURRENT_TIME:
      writeData("CURRENT TIME", formatTime(t));
      display.display();
      nextIdleState = CURRENT_DATE;
      updateIdleTimer(SCREEN_INTERVAL);
      idleState = WAITING;
      break;
    case CURRENT_DATE:
      writeData("CURRENT DATE", formatDate(t));
      display.display();
      nextIdleState = NEXT_ACTION;
      updateIdleTimer(SCREEN_INTERVAL);
      idleState = WAITING;
      break;
    case NEXT_ACTION:
      if (lightState == ON) {
        time_t data = sunrise;
        if (t > sunrise) data = nextSunrise;
        writeData("TURNING OFF AT", formatTime(data));
        display.display();
      } else {
        time_t data = sunset;
        if (t > sunset) data = nextSunset;
        writeData("TURNING ON AT", formatTime(data));
        display.display();
      }
      nextIdleState = CURRENT_TIME;
      updateIdleTimer(SCREEN_INTERVAL);
      idleState = WAITING;
      break;
  }
}

void changeIdleState() {
  idleState = nextIdleState;
}

bool updateButtonRepeat(Bounce *btn, unsigned long *nextTriggerPtr) {
  if (btn->fell()) {
    *nextTriggerPtr = millis() + BUTTON_REPEAT_DELAY;
    return true;
  }
  
  if (btn->read() == LOW && millis() > *nextTriggerPtr) {
    *nextTriggerPtr = millis() + BUTTON_REPEAT_FREQ;
    return true;
  }
  
  return false;
}

void updateMenuState() {
  time_t t = now();
  unsigned long m = millis();

  bool buttonMenuFell = updateButtonRepeat(&buttonMenu, &btnMenuNextTrigger);
  bool buttonUpFell = updateButtonRepeat(&buttonUp, &btnUpNextTrigger);
  bool buttonDownFell = updateButtonRepeat(&buttonDown, &btnDownNextTrigger);
  
  if (buttonMenuFell || buttonUpFell || buttonDownFell) {
    // refresh the menu timeout when a button is pressed
    updateMenuTimer(MENU_TIMEOUT);
  }

  // read inputs and update state
  if (buttonMenuFell) {
    switch (menuState) {
      case SETTINGS_SET_TIME:
        menuState = SETTINGS_SET_DATE;
        break;
      case SETTINGS_SET_DATE:
        menuState = SETTINGS_SET_TIME;
        break;
      case SET_TIME:
        switch (setTimeState) {
          case SET_HOUR:
            setTimeState = SET_MINUTE;
            break;
          case SET_MINUTE:
            displayState = DISPLAY_IDLE;
            idleState = CURRENT_TIME;
            setTime(makeTime(adjustedTime));
            RTC.set(now());
            updateRiseSet();
            break;
        }
        break;
      case SET_DATE:
        switch (setDateState) {
          case SET_YEAR:
            setDateState = SET_MONTH;
            updateRiseSet();
            break;
          case SET_MONTH:
            setDateState = SET_DAY;
            break;
          case SET_DAY:
            displayState = DISPLAY_IDLE;
            idleState = CURRENT_TIME;
            adjustedDate.Hour = hour();
            adjustedDate.Minute = minute();
            adjustedDate.Second = second();
            setTime(makeTime(adjustedDate));
            RTC.set(now());
            updateRiseSet();
            break;
        }
        break;
    }
  }

  if (buttonUpFell || buttonDownFell) {
    int modifier = 1;
    if (buttonDownFell) modifier = -1;
    
    switch(menuState) {
      case SETTINGS_SET_TIME:
        menuState = SET_TIME;
        setTimeState = SET_HOUR;
        breakTime(now(), adjustedTime);
        adjustedTime.Second = 0;
        break;
      case SETTINGS_SET_DATE:
        menuState = SET_DATE;
        setDateState = SET_YEAR;
        breakTime(now(), adjustedDate);
        break;
      case SET_TIME:
        switch (setTimeState) {
          case SET_HOUR:
            adjustedTime.Hour = constrain(adjustedTime.Hour + modifier, 0, 23);
            break;
          case SET_MINUTE:
            adjustedTime.Minute = constrain(adjustedTime.Minute + modifier, 0, 59);
            break;
        }
        break;
      case SET_DATE:
        switch (setDateState) {
          case SET_YEAR:
            adjustedDate.Day = constrain(adjustedDate.Day, 1, countDaysInMonth(adjustedDate.Month, max(adjustedDate.Year + modifier, 50)));
            adjustedDate.Year = max(adjustedDate.Year + modifier, 50);
            break;
          case SET_MONTH:
            adjustedDate.Day = constrain(adjustedDate.Day, 1, countDaysInMonth(constrain(adjustedDate.Month + modifier, 1, 12), adjustedDate.Year));
            adjustedDate.Month = constrain(adjustedDate.Month + modifier, 1, 12);
            break;
          case SET_DAY:
            adjustedDate.Day = constrain(adjustedDate.Day + modifier, 1, countDaysInMonth(adjustedDate.Month, adjustedDate.Year));
            break;
        }
    }
  }

  // render current state
  switch (menuState) {
    case SETTINGS_SET_TIME:
      writeData("SETTINGS", "Set time");
      display.display();
      break;
    case SETTINGS_SET_DATE:
      writeData("SETTINGS", "Set date");
      display.display();
      break;
    case SET_TIME:
      writeData("SET TIME", formatTime(makeTime(adjustedTime)));
      switch (setTimeState) {
        case SET_HOUR:
          if (millis() / 500 % 2 == 0)
            display.drawFastHLine(4, 31, 22, SSD1306_WHITE);
          break;
        case SET_MINUTE:
          if (millis() / 500 % 2 == 0)
            display.drawFastHLine(40, 31, 22, SSD1306_WHITE);
          break;
      }
      display.display();
      break;
    case SET_DATE:
      writeData("SET DATE", formatDateReverse(makeTime(adjustedDate)));
      switch (setDateState) {
        case SET_YEAR:
          if (millis() / 500 % 2 == 0)
            display.drawFastHLine(4, 31, 48, SSD1306_WHITE);
          break;
        case SET_MONTH:
          if (millis() / 500 % 2 == 0)
            display.drawFastHLine(64, 31, 22, SSD1306_WHITE);
          break;
        case SET_DAY:
          if (millis() / 500 % 2 == 0)
            display.drawFastHLine(100, 31, 22, SSD1306_WHITE);
          break;
      }
      display.display();
      break;
  }
}

void closeMenu() {
  displayState = DISPLAY_IDLE;
  idleState = CURRENT_TIME;
}

void updateIdleTimer(int seconds) {
  // cancel the previous timer if it exists
  Alarm.disable(idleTimer);
  Alarm.free(idleTimer);

  // set a new timer for the number of seconds specified and return the id
  idleTimer = Alarm.timerOnce(seconds, changeIdleState);
}

void cancelIdleTimer() {
  // cancel the timer if it exists
  Alarm.disable(idleTimer);
  Alarm.free(idleTimer);
}

void updateMenuTimer(int seconds) {
  // cancel the previous timer if it exists
  Alarm.disable(menuTimer);
  Alarm.free(menuTimer);

  // set a new timer for the number of seconds specified and return the id
  menuTimer = Alarm.timerOnce(seconds, closeMenu);
}

void cancelMenuTimer() {
  // cancel the timer if it exists
  Alarm.disable(menuTimer);
  Alarm.free(menuTimer);
}

void drawSplashScreen() {
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  writeCenter("Settle", SCREEN_WIDTH / 2, 1, 2);
  writeCenter("Hackspace", SCREEN_WIDTH / 2, 17, 2);

  display.display();
}

void writeCenter(const char *buf, int x, int y, int fontSize)
{
  display.setCursor(x - floor(strlen(buf) * ((fontSize * 5) + 1) - 1) / 2, y);
  display.write(buf);
}

void writeData(const char *title, const char *data) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(4, 4);
  display.write(title);

  display.setTextSize(2);
  display.setCursor(4, 14);
  display.write(data);
}

void updateRiseSet() {
  time_t today = now();
  time_t tomorrow = today + 86400;
  bool dstToday = myTZ.locIsDST(today);
  bool dstTomorrow = myTZ.locIsDST(tomorrow);

  // update sunrise and sunset for today
  sunrise = riseSetToTime(today, settle.sunrise(year(), month(), day(), dstToday));
  sunset = riseSetToTime(today, settle.sunset(year(), month(), day(), dstToday));

  // update sunrise and sunset for tomorrow
  nextSunrise = riseSetToTime(tomorrow, settle.sunrise(year(tomorrow), month(tomorrow), day(tomorrow), dstTomorrow));
  nextSunset = riseSetToTime(tomorrow, settle.sunset(year(tomorrow), month(tomorrow), day(tomorrow), dstTomorrow));
}

time_t riseSetToTime(time_t t, int riseSet) {
    tmElements_t timeElements;
    breakTime(t, timeElements);
    timeElements.Second = 0;
    
    timeElements.Hour = floor(riseSet / 60);
    timeElements.Minute = floor(riseSet % 60);
  
    return makeTime(timeElements);
}

int countDaysInMonth(int m, int y) {
  int days = daysPerMonth[m - 1];
  if (m - 1 == 1 && LEAP_YEAR(y)) days++;
  return days;
}

char* formatDate(time_t t) {
  sprintf(dateBuf, "%02d/%02d/%04d", day(t), month(t), year(t));
  return dateBuf;
}

char* formatDateReverse(time_t t) {
  sprintf(dateBuf, "%04d/%02d/%02d", year(t), month(t), day(t));
  return dateBuf;
}

char* formatTime(time_t t) {
  sprintf(timeBuf, "%02d:%02d", hour(t), minute(t));
  return timeBuf;
}

char* formatDateTime(time_t t) {
  sprintf(dateTimeBuf, "%02d/%02d/%02d %02d:%02d", day(t), month(t), year(t), hour(t), minute(t));
  return dateTimeBuf;
}
