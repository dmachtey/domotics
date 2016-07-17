#include "dimmer_ctl.h"
#include <iostream>
//#include <math.h> // for log

#include <cstdio>
#include <cstring>


using namespace std;

#define MIN_SCALING_TIME 1000 // ms to start dimming (less shortpress)
#define SCALING_TIME 500 // ms for each dimming step
#define DIMMING_STEPS 20 // number of dimming steps

namespace lighting{
  unsigned int positive_edge(unsigned int value, unsigned int *mem){
    /* this function is intended to detect the positive edge change of a
       input signal value */
    int out = (value && !(*mem))? 1 : 0;
    *mem = value;
    return out;
  }

  unsigned int negative_edge(unsigned int value, unsigned int *mem){
    /* this function is intended to detect the negative edge change of a
       input signal value */
    int out = (!value && *mem)? 1 : 0;
    *mem = value;
    return out;
  }

  DIMMER::DIMMER(const char *id, const char *host, int port, char *name) : mosquittopp(id){
    off_enable = 0;
    mem_fall = 0;
    mem_rise = 0;
    duty = 0;
    old_duty = 0;
    old_dutyMQ = 0;
    up_down = 0;
    ms = 0;
    ms_g = 0;
    on = 0;
    going_on = 0;
    going_off = 0;
    mqtt_name = name;
    recon_counter = 0;


    int keepalive = 60;
    connect(host, port, keepalive);

    //    cout << "constructor"  <<  endl;
  }

/*
 * | press | is        | start     | on            |
 * |-------+-----------+-----------+---------------|
 * | short | off       | going on  | release       |
 * | short | on        | going_off | release       |
 * | short | going_on  | stop      | release       |
 * | short | going_off | stop      | release       |
 * | long  | off       | fade in   | while pressed |
 * | long  | quiet     | fade out  | while pressed |
 */
  void DIMMER::dloop(unsigned int sw, unsigned int last_loop_ms, unsigned int ring){
    unsigned int shortpress = 0, longpress = 0;
    unsigned int nt_slots = 0;

    longpress = (ms > MIN_SCALING_TIME);
    shortpress = (negative_edge(sw, &mem_fall) && !longpress);

    ms += last_loop_ms; //how long is the sw pressed.
    ms *= (sw>0);

    ms_g += last_loop_ms;
    ms_g *= ((ring+ringing_latch)>0); //(going_on || going_off);

    nt_slots = (ms - MIN_SCALING_TIME)/SCALING_TIME; //for scaling on long press
    if (shortpress && ((going_on || going_off))){
      going_on = 0; //reset
      going_off = 0; //reset
      shortpress = 0;
    }

    if (shortpress && !on && !longpress) going_on = 1; //set
    if (shortpress && on && !longpress) going_off = 1; //set

    if (!going_on && !going_off)
      old_duty = duty;

    if (going_on) goingON(last_loop_ms);
    if (going_off) goingOFF(last_loop_ms);

    if (!sw){
      time_slots = 0; //reset the counter
      up_down = 0; //go down always first
      longpress = 0;
    }

    if (longpress && (time_slots < nt_slots)) //fade
      fadeOUT();

    if(loop(15,1)){
        reconnect();
      }

    // Ring the light
    if (ring || ringing_latch){
      duty = (((ms_g / 350) % 2) == 0)*20+15;
      // cout << duty << " " << ms_g << " last loop " << last_loop_ms << endl;
      ringing = 1;

      ringing_latch = 1;
      if (ms_g > 1500) ringing_latch = 0;
    }
    if (ringing && !ringing_latch) { // go back to the start value TODO control the variables, may be can be included with the time control
      duty = old_dutyMQ;
      ringing = 0;
    }

    if ((duty != old_dutyMQ) && !going_on && !going_off && !ringing_latch){
      old_dutyMQ = duty;

      char buf[51], buf2[10];
      snprintf(buf, 50, "%s/send/duty", mqtt_name);
      snprintf(buf2, 10, "%d", duty);
      publish(NULL, buf, strlen(buf2), buf2);
    }
  }//loop

  void DIMMER::goingON(unsigned int last_loop_ms){
    // duty =  old_duty + 100.0f * ((float)(ms_g)/(float)MIN_SCALING_TIME);
    duty =  duty + (100.0f * ((float)(last_loop_ms)/1000.0f)); // TODO integer calculation
    //duty =  duty + (100 * last_loop_ms/2000);
     on = 1;
    if (duty >= 100){
      duty = 100;
      going_on = 0;
    }
  }

  void DIMMER::goingOFF(unsigned int last_loop_ms){
    //    duty =  old_duty - (100.0f * ((float)(ms_g)/(float)MIN_SCALING_TIME));
    duty =  duty - (100.0f * ((float)(last_loop_ms)/1000.0f)); // TODO integer calculation
    // cout << "going off ms " << ms_g << " " << duty << " added " << (100.0f * ((float)(last_loop_ms)/2000.0f)) << endl;
    if (duty <= 0){
      duty = 0;
      on = 0;
      going_off = 0;
    }
  }

  void DIMMER::fadeOUT(){
    time_slots += 1;
    if (up_down){ //up
      duty += 100.0f/DIMMING_STEPS;
      if (duty >= 100){
        duty = 100;
        up_down = 0;
      }
    } // up
    else{ // down
      duty -= 100.0f/DIMMING_STEPS;
      if (duty <= 0){
        duty = 100.0f/DIMMING_STEPS;
        up_down = 1;
      }
    } // down
    on = duty > 0;
  } // dimming


void DIMMER::on_connect(int rc)
{
	printf("Connected with code %d.\n", rc);
	if(rc == 0){
      /* Only attempt to subscribe on a successful connect. */
      char buf[51];
      snprintf(buf, 50, "%s/get/#", mqtt_name);
      //   printf("%s\n", buf);
      subscribe(NULL, buf);
	}
}

void DIMMER::on_message(const struct mosquitto_message *message)
{
  char buf[51];
  snprintf(buf, 50, "%s/get/duty", mqtt_name);
  //  printf("got the message: %s for %s, ", message->payload, mqtt_name);

  //        if(!going_on || !going_off)
  if(!strcmp(message->topic, buf)){
    memset(buf, 0, 51*sizeof(char));
    /* Copy N-1 bytes to ensure always 0 terminated. */
    memcpy(buf, message->payload, 50*sizeof(char));
    duty = atof(buf);
    on = duty > 0;
    // printf("setting duty at %d\n", duty);
  }
}

void DIMMER::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
	printf("Subscription succeeded.\n");
}



  DIMMER::~DIMMER(){
    off_enable = 0;
  }
} /* namespace lighting */
