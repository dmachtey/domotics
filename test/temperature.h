#ifndef temperature_H_
#define temperature_H_
#include <string>

#include <mosquittopp.h>
#include <stdlib.h>

using std::string;

namespace lighting{

  class TEMPERATURE : public mosqpp::mosquittopp{
   private:
    string temp;
    unsigned int time_ac; // time accumulator
    unsigned int time_at; // publish
    unsigned int first_cycle;
    string topic;
    string mqtt_name;
    string location; // file location
  public:
    TEMPERATURE(const char *id, const char *host, int port, char *name, char *where,
                unsigned int publish_every_ms);
    void tloop(unsigned int last_loop_ms);
    string getTemp() {return temp;}
    double getTempd() {return atof(temp.c_str());}
    ~TEMPERATURE();
  private:
    void read();
    void publishnow();
    void on_connect(int rc);
    void on_message(const struct mosquitto_message *message);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);
  };
} /* namespace lighting */

#endif /* temperature_H_ */
