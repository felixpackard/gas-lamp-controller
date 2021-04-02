#ifndef SET_DATE_SCREEN_H
#define SET_DATE_SCREEN_H

#include <Time.h>
#include <DS1307RTC.h>

#include "../screen.h"
#include "../../light_service/light_service.h"

namespace light_controller {

  enum SetDateScreenState {
    SET_YEAR,
    SET_MONTH,
    SET_DAY
  };

  class SetDateScreen : public AbstractScreen {
    
  public:
    // Constructor
    SetDateScreen(Adafruit_SSD1306 &display) : AbstractScreen(display, 2), light_service(LightService::get_instance()) { };

    // Destructor
    ~SetDateScreen() = default;

    void init();
    void update(bool menu_pressed, bool down_pressed, bool up_pressed);
    void render();

  private:
    void set_data(const char *new_data);
    
    const char *title = "Set date";
    char data[16];
    
    LightService &light_service;
    tmElements_t adjusted_date;

  };

};

#endif
