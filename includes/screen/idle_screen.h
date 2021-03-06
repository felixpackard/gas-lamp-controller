#ifndef IdleScreen_H
#define IdleScreen_H

#include "screen.h"
#include "../config.h"

namespace light_controller {

  class IdleScreen : public AbstractScreen {

  public:
    // Constructor
    IdleScreen(Adafruit_SSD1306 &display) : AbstractScreen(display, 1) { };

    // Destructor
    ~IdleScreen() = default;

    void init();
    void update(bool menu_pressed, bool down_pressed, bool up_pressed);
    void render();

  private:
    void set_title(const char *new_title);

    enum IdleScreenState {
      CURRENT_TIME,
      CURRENT_DATE
      // TODO: Complete enum
    };
    
    char title[16];
    char data[16];

  };

};

#endif
