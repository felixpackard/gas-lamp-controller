#ifndef IdleScreen_H
#define IdleScreen_H

#include <assert.h>

#include "screen.h"

namespace light_controller {

  class IdleScreen : public AbstractScreen {

  public:
    // Constructor
    IdleScreen(Adafruit_SSD1306 &display) : AbstractScreen(display) { };

    // Destructor
    ~IdleScreen() = default;

    void update(bool menu_pressed, bool down_pressed, bool up_pressed);
    void render();

  private:
    void set_title(const char *new_title);

    enum IdleScreenState {
      CURRENT_TIME,
      CURRENT_DATE
      // TODO: Complete enum
    };
    
    const uint8_t max_state = 1;
    char title[16];
    char data[16];

  };

};

#endif
