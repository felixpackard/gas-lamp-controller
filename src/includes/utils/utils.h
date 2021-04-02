#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <Time.h>
#include <TimeAlarms.h>

#include "../config.h"

namespace light_controller {

  typedef void (*TimerEvent)();

  /**
   * @brief Refers to the severity level of a log message.
   * 
   */
  enum LogLevel {
    INFO,
    WARNING,
    ERROR
  };

  const char * const LogLevelString[] = {
    "[INFO] ",
    "[WARN] ",
    "[ERR] "
  };

  const uint8_t days_per_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  #define leap_year(y) ( ((1970+(y))>0) && !((1970+(y))%4) && ( ((1970+(y))%100) || !((1970+(y))%400) ) )

  #define format_time(buffer, t) snprintf_P(buffer, sizeof(buffer) - 1, PSTR("%02d:%02d"), hour(t), minute(t))
  #define format_date(buffer, t) snprintf_P(buffer, sizeof(buffer) - 1, PSTR("%02d/%02d/%04d"), day(t), month(t), year(t))
  #define format_date_reverse(buffer, t) snprintf_P(buffer, sizeof(buffer) - 1, PSTR("%04d/%02d/%02d"), year(t), month(t), day(t))
  #define set_string(dest, src) strlcpy_P(dest, src, strlen_P(src) + 1)

  class Utils {

  public:
    /**
     * @brief Start a new timer
     * 
     * @param timer a pointer to the variable in which the ID of the timer should be stored
     * @param seconds the number of seconds to set the timer for
     * @param cb the function to call once the time has elapsed
     */
    static AlarmID_t set_timer(AlarmID_t timer, int seconds, TimerEvent cb);

    /**
     * @brief Print a debug message to the Serial output
     * @note only prints the message if DEBUG is enabled
     * @see config.h
     * 
     * @param severity the severity level of the message
     * @param message the message to write to the Serial output
     */
    static void debug(LogLevel severity, const char *message);

  };

};

// namespace light_controller {
//   class Utils {
    
//   public:
//     static time_t riseSetToTime(time_t t, int riseSet) {
//       tmElements_t timeElements;
//       breakTime(t, timeElements);

//       timeElements.Second = 0;
      
//       timeElements.Hour = floor(riseSet / 60);
//       timeElements.Minute = floor(riseSet % 60);
    
//       return makeTime(timeElements);
//     }

//     static int countDaysInMonth(int m, int y) {
//       int days = daysPerMonth[m - 1];
//       if (m - 1 == 1 && LEAP_YEAR(y)) days++;
//       return days;
//     }

//     static void formatDate(char* buffer, time_t t) {
//       sprintf(buffer, "%02d/%02d/%04d", day(t), month(t), year(t));
//     }

//     static void formatDateReverse(char* buffer, time_t t) {
//       sprintf(buffer, "%04d/%02d/%02d", year(t), month(t), day(t));
//     }

//     static void formatTime(char* buffer, time_t t) {
//       sprintf(buffer, "%02d:%02d", hour(t), minute(t));
//     }

//     static void formatDateTime(char* buffer, time_t t) {
//       sprintf(buffer, "%02d/%02d/%02d %02d:%02d", day(t), month(t), year(t), hour(t), minute(t));
//     }
    
//   };
// }

#endif
