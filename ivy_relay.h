#ifndef IVY_H
#define IVY_H

#include "Arduino.h"
#include "DHT.h"
/*
  This is your hardware controller. Let us handle the tricky bits regarding relay safety, you handle the rest!
*/

const int ON = 1;
const int OFF = 0;
void ivy_begin();
void ivy_setOutlet(unsigned int outlet, unsigned int state);
unsigned int ivy_getOutlet(unsigned int outlet);
float ivy_getHumidity();
float ivy_getTemperature();

#endif