#ifndef IDLESCREEN_H
#define IDLESCREEN_H

#include "screen.h"
#include "../config.h"
#include "../utils/utils.h"
#include "../light_service/light_service.h"

namespace light_controller {

  class IdleScreen : public AbstractScreen {

  public:
    // Constructor
    IdleScreen(Adafruit_SSD1306 &display) : AbstractScreen(display, 2), light_service(LightService::get_instance()) { };

    // Destructor
    ~IdleScreen() = default;

    void init();
    void update(bool menu_pressed, bool down_pressed, bool up_pressed);
    void render();

  private:
    void set_title(const char *new_title);

    enum IdleScreenState {
      CURRENT_TIME,
      CURRENT_DATE,
      NEXT_ACTION
    };
    
    char title[16];
    char data[16];

    LightService &light_service;

  };

};

#endif
