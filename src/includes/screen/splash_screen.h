#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <Adafruit_SSD1306.h>

#include "screen.h"

namespace light_controller {

  class SplashScreen : public AbstractScreen {

  public:
    // Constructor
    SplashScreen(Adafruit_SSD1306 &display) : AbstractScreen(display) { };

    // Destructor
    ~SplashScreen() = default;

    void render();

  };

};

#endif
