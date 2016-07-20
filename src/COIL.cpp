#include "COIL.h"

// Constructors/Destructors
//  

COIL::COIL () {
initAttributes();
}

COIL::~COIL () { }

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  

void COIL::initAttributes () {
  on = false;
  time_off_acc = 0;
  sw_press_acc = 0;
  publish_acc = 0;
  publish_acc_sp = 0;
  power = 0;
  actual_power = 0.0;
  total_power = 0.0;
  total_power_all_clases = 0.0;
}

