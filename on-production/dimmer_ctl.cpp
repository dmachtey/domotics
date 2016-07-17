#include "dimmer_ctl.h"
#include <iostream> //cin cout cerr (for errors) clog (for logs)
#include <cstring> // memcpy, strcmp, strlen...

using namespace std;

#define MIN_SCALING_TIME 1000 // ms to start dimming (less shortpress)
#define SCALING_TIME 500 // ms for each dimming step
#define DIMMING_STEPS 20 // number of dimming steps
#define TIME_OUT_PUBLISH 2*60*1000 // 2 minutes

namespace lighting{
  /*
   * uint positive_edge(uint value, uint *mem){
   *   /\* this function is intended to detect the positive edge change of a
   *      input signal value *\/
   *   int out = (value && !(*mem))? 1 : 0;
   *   *mem = value;
   *   return out;
   * }
   *
   * uint negative_edge(uint value, uint *mem){
   *   /\* this function is intended to detect the negative edge change of a
   *      input signal value *\/
   *   int out = (!value && *mem)? 1 : 0;
   *   *mem = value;
   *   return out;
   * }
   */



  DIMMER::DIMMER(const char *id, const char *host, int port, char *name,
                 uint mstimeout) : mosquittopp(id){
    on = false;
    duty = 0;
    old_dutyMQ = 0;
    up_down = false;
    ms = 0;
    ms_g = 0;
    GoingOn = false;
    going_off = false;
    mqtt_name = name;
    publish_ms = rand()%(TIME_OUT_PUBLISH);
    ringing_latch = false;
    ringing = false;
    int keepalive = 60;
    connect(host, port, keepalive);
    publishnow();
    max_level = 100;
    ms_timeout = mstimeout;
  }

  /*
   * | press | is        | start     | on            |
   * |-------+-----------+-----------+---------------|
   * | short | off       | going on  | release       |
   * | short | on        | going_off | release       |
   * | short | GoingOn  | stop      | release       |
   * | short | going_off | stop      | release       |
   * | long  | off       | fade in   | while pressed |
   * | long  | quiet     | fade out  | while pressed |
   */
  void DIMMER::dloop(bool sw, uint last_loop_ms, bool ring){
    bool ShortPress = false, LongPress = false;
    uint NrOfSlots = 0;

    LongPress = (ms > MIN_SCALING_TIME);
    ShortPress = ((ms>0) && !sw && !LongPress);

    ms += last_loop_ms; //how long is the sw pressed.
    ms *= (sw>0);

    ms_g += last_loop_ms;
    ms_g *= ((ring+ringing_latch)>0);

    ms_timeoutC += last_loop_ms;
    ms_timeoutC *= (on && (ms_timeout > 0) && !sw); // reset the counter if not on or timeout <= 0
    if (ms_timeoutC > ms_timeout)
      going_off = true;

    publish_ms += last_loop_ms;
    ms_going_on_off += last_loop_ms; //Steps at automatic fadding

    NrOfSlots = (ms - MIN_SCALING_TIME)/SCALING_TIME; //for scaling on long press
    if (ShortPress && ((GoingOn || going_off))){
      GoingOn = false; //reset
      going_off = false; //reset
      ShortPress = false;
    }

    if (ShortPress && !on) GoingOn = true; //set
    if (ShortPress && on) going_off = true; //set


    if (ms_going_on_off > 12){
    if (GoingOn) goingON();
    if (going_off) goingOFF();
    ms_going_on_off = 0;
    }


    if (!sw){
      time_slots = 0; //reset the counter
      up_down = false; //go down always first
      LongPress = false;
    }

    if (LongPress && (time_slots < NrOfSlots)) //fade
      fadeOUT();

    if(loop(15,1)){
      reconnect();
    }

    // Ring the light
    if (ring || ringing_latch){
      duty = (((ms_g / 350) % 2) == 0)*70+15;
      ringing = true;

      ringing_latch = true;
      if (ms_g > 3000) ringing_latch = 0;
    }
    if (ringing && !ringing_latch) { // go back to the start value
      duty = old_dutyMQ;
      ringing = false;
    }

    if (((duty != old_dutyMQ) || publish_ms > TIME_OUT_PUBLISH) && !GoingOn && !going_off && !ringing_latch){
      old_dutyMQ = duty;
      publish_ms *= (publish_ms < TIME_OUT_PUBLISH); //Reset the counter when published was forced
      publishnow();
    }

    if (duty >= max_level) {
      duty = max_level;
      on = true;
        }
    if (duty <= 0) {
      duty = 0;
      on = false;
        }
  }//loop

  void DIMMER::goOffNow(){
    duty = max_level;
    GoingOn = false;
    going_off = false;
    on = true;
    publishnow();
  }

  void DIMMER::goOnNow(){
    duty = false;
    GoingOn = false;
    going_off = false;
    on = false;
    publishnow();
  }

  void DIMMER::goOff(){
    if (!GoingOn && on)
      going_off = true;
  }

  void DIMMER::goOn(){
    if (!going_off && !on)
      GoingOn = true;
  }

  void DIMMER::goingON(){
    duty += (6*max_level)/100;
    on = true;
    if (duty >= max_level){
      duty = max_level;
      GoingOn = false;
    }
  }

  void DIMMER::goingOFF(){
    duty -= (6*max_level)/100;
    if (duty <= ((5*max_level)/100)){
      duty = 0;
      on = false;
      going_off = false;
    }
  }

  void DIMMER::fadeOUT(){
    time_slots += 1;
    if (up_down){ //up
      duty += max_level/DIMMING_STEPS;
      if (duty >= max_level){
        duty = max_level;
        up_down = false;
      }
    } // up
    else{ // down
      duty -= max_level/DIMMING_STEPS;
      if (duty <= 0){
        duty = max_level/DIMMING_STEPS;
        up_down = true;
      }
    } // down
    on = duty > 0;
  } // dimming

  void DIMMER::on_connect(int rc)
  {
	cout << "Connected with code: " << rc << " from DIMMER::" << mqtt_name << endl;
	if(rc == 0){
      /* Only attempt to subscribe on a successful connect. */
      string sub = mqtt_name + "/get/#";
      subscribe(NULL, sub.c_str());
	}
  }

  void DIMMER::on_message(const struct mosquitto_message *message)
  {
    string search_msg = mqtt_name + "/get/duty";
    if(!strcmp(message->topic, search_msg.c_str())){
      duty = atof((char *)message->payload);
      on = duty > 0;
      ms_timeoutC = 0;
    }
  }

  void DIMMER::on_subscribe(int mid, int qos_count, const int *granted_qos)
  {
	cout << "Subscription succeeded from DIMMER::" << mqtt_name << endl;
  }

  void DIMMER::publishnow()
  {
    string topic = mqtt_name + "/send/duty";
    string payload = to_string(duty);  // CFLAGS=-std=c++11 on Makefile
    publish(NULL, topic.c_str(), payload.length() , payload.c_str());
  }

  DIMMER::~DIMMER(){
    cout << "destroying DIMMER::" << mqtt_name << endl;
  }
} /* namespace lighting */
