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
     * @brief Returns a number of days based on the month and year provided
     * 
     * @param month the month to check the number of days for
     * @param year the year to check the number of days in the month for
     * @return int the number of days in the month
     */
    static uint8_t count_days_in_month(uint8_t month, uint8_t year);

  };

};

#endif
