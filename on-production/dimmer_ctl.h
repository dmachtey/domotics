#ifndef dimmer_ctl_H_
#define dimmer_ctl_H_
#include <string>

#include <mosquittopp.h>

using std::string;

namespace lighting{

  unsigned int positive_edge(unsigned int value, unsigned int *mem);
  unsigned int negative_edge(unsigned int value, unsigned int *mem);



  class DIMMER : public mosqpp::mosquittopp{
   private:
    unsigned int on; // the light is on
    unsigned int off_enable; // we may switch it off (on switch release)
    unsigned int duty; // actual duty
    unsigned int old_duty; // for fadeout
    unsigned int old_dutyMQ;
    unsigned int up_down;  // dimmering up and down
    unsigned int mem_rise; // switch rising edge
    unsigned int mem_fall; // switch falling edge
    unsigned int ms; // switch pressed milliseconds
    unsigned int ms_g;
    unsigned int time_slots; // to count how many slots is the sw pressed
    unsigned int going_on;
    unsigned int going_off;
    unsigned int ringing;
    unsigned int ringing_latch;
    char *mqtt_name;
    unsigned int recon_counter;
  public:
    DIMMER(const char *id, const char *host, int port, char *name);
    void dloop(unsigned int sw, unsigned int last_loop_ms, unsigned int ring);
    unsigned int getDuty() {return duty;}
    ~DIMMER();
  private:
    void goingON(unsigned int last_loop_ms);
    void goingOFF(unsigned int last_loop_ms);
    void fadeIN();
    void fadeOUT();
    void on_connect(int rc);
    void on_message(const struct mosquitto_message *message);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);

  };
} /* namespace lighting */

#endif /* dimmer_ctl_H_ */
