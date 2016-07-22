
#ifndef DIMMER_H
#define DIMMER_H
#include "lighting/COIL.h"

#include <string>

namespace lighting {

class DIMMER : public COIL
{
public:


protected:

  // Protected attributes
  //  

  unsigned int duty;
  unsigned int old_duty;
public:

protected:

public:

protected:


private:

  // Private attributes
  //  

  bool up_down;
  unsigned int max_level;
  unsigned int sw_slots;
  unsigned int ringing_acc;
  unsigned int fadding_acc;
  bool ringing;
  bool ringing_latch;
  bool going_on;
  bool going_off;
public:

private:

public:

private:



};
}; // end of package namespace

#endif // DIMMER_H
