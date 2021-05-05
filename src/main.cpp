#include "main.h"
using namespace light_controller;

Bounce bounce_menu = Bounce();
Bounce bounce_down = Bounce();
Bounce bounce_up = Bounce();

Button button_menu = Button(&bounce_menu);
Button button_down = Button(&bounce_down);
Button button_up = Button(&bounce_up);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, 4);

AlarmID_t daily_alarm;
AlarmID_t splash_screen_timer;

LightService &light_service = LightService::get_instance();

AbstractScreen *current_screen = nullptr;

SplashScreen splash_screen(display);
IdleScreen idle_screen(display);
SettingsScreen settings_screen(display);
SetTimeScreen set_time_screen(display);
SetDateScreen set_date_screen(display);
SetModeScreen set_mode_screen(display);

void setup() {
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
    while (1);
  }

  /*
   * Initialize RTC
   */
  setSyncProvider(RTC.get);

  if (timeStatus() != timeSet) {
    RTC.set((unsigned long) DEFAULT_TIME);
    setTime((unsigned long) DEFAULT_TIME);
  }

  /*
   * Initialize LightService
   */
  light_service.update_rise_set();
  Utils::set_repeating_alarm(daily_alarm, DAILY_UPDATE_HOURS, DAILY_UPDATE_MINUTES, DAILY_UPDATE_SECONDS, light_controller::daily_alarm_triggered);
  // daily_alarm = Alarm.alarmRepeat(0, 0, 5, light_controller::daily_alarm_triggered);

  /*
   * Initialize menus
   */
  idle_screen.on_button_pressed(light_controller::idle_button_pressed);

  settings_screen.on_button_pressed(light_controller::settings_button_pressed);
  settings_screen.on_timeout(light_controller::show_idle);

  set_time_screen.on_timeout(light_controller::update_daily_alarm);
  set_date_screen.on_timeout(light_controller::update_daily_alarm);
  set_mode_screen.on_timeout(light_controller::show_idle);

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

void light_controller::settings_button_pressed(bool menu_pressed, bool down_pressed, bool up_pressed) {
  if (down_pressed || up_pressed) {
    switch (settings_screen.get_state()) {
      case SettingsScreenState::SET_TIME:
        set_time_screen.init();
        current_screen = &set_time_screen;
        break;
      case SettingsScreenState::SET_DATE:
        set_date_screen.init();
        current_screen = &set_date_screen;
        break;
      case SettingsScreenState::SET_MODE:
        set_mode_screen.init();
        current_screen = &set_mode_screen;
        break;
    }
  }
}

void light_controller::update_daily_alarm() {
  /*
   * Restart the repeating alarm after the time has been set, or it won't be triggered
   */
  Utils::set_repeating_alarm(daily_alarm, DAILY_UPDATE_HOURS, DAILY_UPDATE_MINUTES, DAILY_UPDATE_SECONDS, light_controller::daily_alarm_triggered);
  show_idle();
}

void light_controller::daily_alarm_triggered() {
  light_service.update_rise_set();
}
