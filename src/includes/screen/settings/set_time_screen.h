#ifndef SETTIMESCREEN_H
#define SETTIMESCREEN_H

#include <Time.h>
#include <DS1307RTC.h>

#include "../screen.h"
#include "../../light_service/light_service.h"

namespace light_controller {

  enum SetTimeScreenState {
    SET_HOUR,
    SET_MINUTE
  };

  class SetTimeScreen : public AbstractScreen {
    
  public:
    // Constructor
    SetTimeScreen(Adafruit_SSD1306 &display) : AbstractScreen(display, 1), light_service(LightService::get_instance()) { };

    // Destructor
    ~SetTimeScreen() = default;

    void init();
    void update(bool menu_pressed, bool down_pressed, bool up_pressed);
    void render();

  private:
    void set_data(const char *new_data);
    
    const char *title = "Set time";
    char data[16];
    
    LightService &light_service;
    tmElements_t adjusted_time;

  };

};

#endif
