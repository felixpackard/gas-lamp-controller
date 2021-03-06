#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <Bounce2.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "../config.h"

// namespace light_controller {

//   // using ButtonPressEvent = bool (*)();
//   typedef bool (*ButtonPressEvent)(bool);

//   /**
//    * @brief Refers to a specific physical Button.
//    * 
//    */
//   enum Button {
//     MENU,
//     DOWN,
//     UP
//   };
  
//   /**
//    * @brief The Screen class. Represents a title and some data to be displayed.
//    * 
//    */
//   class Screen {

//     friend class Menu;

//   public:
//     /**
//      * @brief Construct a new Screen object
//      * 
//      */
//     Screen() = default;

//     /**
//      * @brief Construct a new Screen object
//      * 
//      * @param title the title to display
//      */
//     Screen(const char *title) : _title(title) { }

//     /**
//      * @brief Construct a new Screen object
//      * 
//      * @param title the title to display
//      * @param data the data to display
//      */
//     Screen(const char *title, const char *data) : _title(title), _data(data) { }

//     /**
//      * @brief Render the Screen object
//      * 
//      * @param display a pointer to the display object
//      */
//     void render(Adafruit_SSD1306 &display);
  
//   private:
//     /**
//      * @brief Update the Screen object
//      * 
//      * @param button_menu a pointer to the debounce instance for the menu button
//      * @param button_down a pointer to the debounce instance for the down button
//      * @param button_up a pointer to the debounce instance for the up button
//      * @return whether a button press was handled
//      */
//     bool update(Bounce &button_menu, Bounce &button_down, Bounce &button_up);

//     /**
//      * @brief Write a title and some data to the display
//      * 
//      * @param display a pointer to the display object
//      * @param title the title to display
//      * @param data the data to display
//      */
//     void write_data(Adafruit_SSD1306 &display, const char *title, const char *data);

//     /**
//      * @brief Register a screen to be activated when a specific button is pressed
//      * 
//      * @param button the button to register the action against
//      * @param screen the screen to active when the button is pressed
//      */
//     void on_press(Button button, Screen &screen);

//     /**
//      * @brief 
//      * 
//      * @param button the button to register the action against
//      * @param cb the function to call when the button is pressed
//      */
//     void on_press(Button button, ButtonPressEvent cb);

//     const char *_title;
//     const char *_data;

//     ButtonPressEvent _cb_menu;
//     ButtonPressEvent _cb_down;
//     ButtonPressEvent _cb_up;
    
//   };

//   /**
//    * @brief The Menu class. Represents a collection of screens.
//    * 
//    */
//   class Menu {  

//   public:
//     /**
//      * @brief Construct a new Menu object
//      * 
//      */
//     Menu() = default;

//     /**
//      * @brief Add a Screen object to the Menu
//      * 
//      * @param screen a pointer to the screen that should be added
//      * @return whether the screen was added successfully
//      */
//     bool add_screen(Screen &screen);

//     /**
//      * @brief Update the Menu object
//      * 
//      * @param button_menu a pointer to the debounce instance for the menu button
//      * @param button_down a pointer to the debounce instance for the down button
//      * @param button_up a pointer to the debounce instance for the up button
//      * @return whether a button press was handled
//      */
//     bool update(Adafruit_SSD1306 &display, Bounce &button_menu, Bounce &button_down, Bounce &button_up);

//     /**
//      * @brief Render the active Screen object
//      * 
//      * @param display a pointer to the display object
//      */
//     void render(Adafruit_SSD1306 &display);
  
//   private:
//     Screen *_screens[MAX_SCREENS];
//     uint8_t _screen_count = 0;
//     uint8_t _active_screen = 0;

//   };

// };

#endif
