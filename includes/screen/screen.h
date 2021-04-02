#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "../utils/utils.h"

namespace light_controller {

  typedef uint8_t ScreenState;
  typedef void (*ButtonPressEvent)(bool, bool, bool);
  typedef void (*TimeoutEvent)();

  class AbstractScreen {

  public:
    // Constructor
    AbstractScreen(Adafruit_SSD1306 &display) : display(display) { };
    AbstractScreen(Adafruit_SSD1306 &display, uint8_t max_state) : max_state(max_state), display(display) { };

    // Destructor
    virtual ~AbstractScreen() = default;

    virtual void update(bool menu_pressed, bool down_pressed, bool up_pressed);
    virtual void render() = 0;

    void on_button_pressed(ButtonPressEvent cb);
    void on_timeout(TimeoutEvent cb);

    ScreenState get_state();
    void init();
  
  protected:
    void next_state();
    void write_center(const char *buf, int x, int y, int font_size);
    void write_data(const char *title, const char *data);

    ButtonPressEvent cb_button_pressed;
    TimeoutEvent cb_timeout;

    const uint8_t max_state = 0;

    Adafruit_SSD1306 &display;
    ScreenState state;

    unsigned long timer;

  };

};

#endif
