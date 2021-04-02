#ifndef SET_MODE_SCREEN_H
#define SET_MODE_SCREEN_H

#include <Time.h>
#include <DS1307RTC.h>

#include "../screen.h"
#include "../../light_service/light_service.h"

namespace light_controller {

  class SetModeScreen : public AbstractScreen {
    
  public:
    // Constructor
    SetModeScreen(Adafruit_SSD1306 &display) : AbstractScreen(display, 2), light_service(LightService::get_instance()) { };

    // Destructor
    ~SetModeScreen() = default;

    void init();
    void update(bool menu_pressed, bool down_pressed, bool up_pressed);
    void render();

  private:
    void set_data(const char *new_data);
    
    const char *title = "Operation mode";
    char data[16];
    
    LightService &light_service;

  };

};

#endif
