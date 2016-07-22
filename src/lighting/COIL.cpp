#include "COIL.h"

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  


/**
 * COIL Constructor
 * @param  id
 * @param  host
 * @param  port
 * @param  name
 * @param  auto_off_time
 * @param  power
 */
 COIL::COIL (const char* id, const char* host, int port, const char* name, unsigned int auto_off_time, unsigned int power)
{
}


/**
 * This function must be called in an infinite loop
 * It take care of all the functionality of the class
 * 
 * @return bool
 * @param  scan_time
 * @param  sw
 */
bool COIL::looop (unsigned int scan_time, bool sw)
{
}


/**
 * This function is the implementation of mosquittopp
 * Its call when connections is stablished
 * 
 * @param  rc
 */
void COIL::on_connect (int rc)
{
}


/**
 * This function is the implementation of mosquittopp
 * is called every time a message is received
 * 
 * @param  message
 */
void COIL::on_message (struct const mosquitto_message* message)
{
}


/**
 * This function is the implementation of mosquittopp
 * is called after subscription
 * 
 * @param  mid
 * @param  qos_count
 * @param  granted_qos
 */
void COIL::on_subscribe (int mid, int qos_count, const int* granted_qos)
{
}


/**
 * Publish object status to moquitto server
 */
void COIL::publish_now ()
{
}


/**
 * Compute the consumed power
 * 
 * @param  scan_time
 */
void COIL::power (unsigned int scan_time)
{
}


