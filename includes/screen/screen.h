#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "../utils/utils.h"

namespace light_controller {

  typedef uint8_t ScreenState;
  typedef void (*ButtonPressEvent)(bool, bool, bool);

  class AbstractScreen {

  public:
    // Constructor
    AbstractScreen(Adafruit_SSD1306 &display) : display(display) { };

    // Destructor
    virtual ~AbstractScreen() = default;

    virtual void update(bool menu_pressed, bool down_pressed, bool up_pressed);
    virtual void render() = 0;

    void on_button_pressed(ButtonPressEvent cb);
    void on_down_pressed(ButtonPressEvent cb);
    void on_up_pressed(ButtonPressEvent cb);

    ScreenState get_state();
  
  protected:
    void write_center(const char *buf, int x, int y, int font_size);
    void write_data(const char *title, const char *data);

    ButtonPressEvent cb_button_pressed;

    const uint8_t max_state = 0;

    Adafruit_SSD1306 &display;
    ScreenState state;

  };

};

#endif
