#ifndef IVY_RELAY_H
#define IVY_RELAY_H

#include "Arduino.h"

const int N_OUTLETS = 8;
unsigned int outlet_state[N_OUTLETS];
const int ON = 1;
const int OFF = 0;
void ivy_relay_begin();
void ivy_updateOutlets();

const int OUTLET_PIN[] = { 16, 17, 18, 19, 21, 22, 23, 25 };

void ivy_relay_begin(){
  for(int outlet = 0; outlet < N_OUTLETS; outlet++){
    pinMode(OUTLET_PIN[outlet], OUTPUT);
    digitalWrite(OUTLET_PIN[outlet], 1);
    outlet_state[outlet] = 0;
  }
}

void ivy_updateOutlets(){
  for(int i = 0; i < N_OUTLETS; i++){
    digitalWrite(OUTLET_PIN[i], (outlet_state[i] != 0) ? 0 : 1);
  } 
}

#endif