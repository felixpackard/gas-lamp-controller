#ifndef LIGHTSERVICE_H
#define LIGHTSERVICE_H

#include <Time.h>
#include <TimeAlarms.h>
#include <Timezone.h>
#include <Dusk2Dawn.h>

#include "../config.h"

namespace light_controller {

  enum LightState {
    ON,
    OFF
  };

  class LightService {

  public:
    static LightService &get_instance();
    void update_rise_set();
    void update();

    LightState get_state();
    time_t get_next_action_time();
  
  private:
    LightService() : my_tz(tcr_dst, tcr_std), settle(LATITUDE, LONGITUDE, UTC_OFFSET) { };

    time_t rise_set_to_time(time_t t, uint8_t riseSet);
    
    TimeChangeRule tcr_dst = DST_START;
    TimeChangeRule tcr_std = STD_START;
    
    Timezone my_tz;
    Dusk2Dawn settle;

    time_t sunrise;
    time_t sunset;
    time_t next_sunrise;
    time_t next_sunset;

    LightState state;
    time_t next_action_time;

    uint8_t brightness;
    uint8_t bri_multiplier;
    unsigned long bri_next_update;

  };

};

#endif
