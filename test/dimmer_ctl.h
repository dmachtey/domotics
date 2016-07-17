#ifndef dimmer_ctl_H_
#define dimmer_ctl_H_
#include <string>

#include <mosquittopp.h>

using std::string;

namespace lighting{
  typedef unsigned int uint;


  class DIMMER : public mosqpp::mosquittopp{
  private:
    bool on; // the light is on
    uint duty; // actual duty
    uint old_dutyMQ;
    bool up_down;  // dimmering up and down
    uint ms; // switch pressed milliseconds
    uint ms_g;
    uint ms_going_on_off; // for going on and off
    uint publish_ms;
    uint time_slots; // to count how many slots is the sw pressed
    bool going_on;
    bool going_off;
    bool ringing;
    bool ringing_latch;
    uint ms_timeout; // off after not been used, if 0 not timeout
    uint ms_timeoutC; //time off counter
    uint max_level;
    string mqtt_name;

    void goingON();
    void goingOFF();
    //void fadeIN();
    void fadeOUT();
    void on_connect(int rc);
    void on_message(const struct mosquitto_message *message);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);
    void publishnow();

  public:
    DIMMER(const char *id, const char *host, int port, char *name,
           uint mstimeout);
    void Dloop(bool sw, uint last_loop_ms, bool ring);
    uint getDuty() {return duty;}
    bool isOn() {return on;}
    void goOffNow();
    void goOnNow();
    void goOff(); // fadding
    void goOn(); // fadding
    void setMaxLevel(uint maxLevel) {max_level = maxLevel;}
    void setTimeOut(uint mstimeout) {ms_timeout = mstimeout;}
    uint getTimeOut() {return ms_timeout;}
    uint getMaxLevel() {return max_level;}
    void setDuty(uint dutY) {duty = dutY;}
    ~DIMMER();

  };
} /* namespace lighting */

#endif /* dimmer_ctl_H_ */
