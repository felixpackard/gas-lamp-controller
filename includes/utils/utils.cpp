#include "utils.h"
using namespace light_controller;

AlarmID_t Utils::set_timer(AlarmID_t timer, int seconds, TimerEvent cb) {
  Alarm.disable(timer);
  Alarm.free(timer);

  return Alarm.timerOnce(seconds, cb);
}

void Utils::debug(LogLevel severity, const char *message) {
  if (!SERIAL_DEBUG) return;

  Serial.print(LogLevelString[severity]);
  Serial.println(message);
}
