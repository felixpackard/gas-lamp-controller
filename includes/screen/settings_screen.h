#ifndef SETTINGSSCREEN_H
#define SETTINGSSCREEN_H

#include "screen.h"

namespace light_controller {

  class SettingsScreen : public AbstractScreen {
  
  public:
    // Constructor
    SettingsScreen(Adafruit_SSD1306 &display) : AbstractScreen(display) { };

    // Destructor
    ~SettingsScreen() = default;

    void update(bool menu_pressed, bool down_pressed, bool up_pressed);
    void render();

  private:
    void set_data(const char *new_data);

    enum IdleScreenState {
      SET_TIME,
      SET_DATE,
      SET_PWD
    };
    
    const uint8_t max_state = 2;
    const char *title = "Settings";
    char data[16];

  };

};

#endif
