#include "ivy_relay.h"

auto current_millis = millis();
auto last_dht_read = current_millis;
float ivy_humidity = 0;
float ivy_temperature = 0;

DHT dht(4, DHT11);

const int N_OUTLETS = 8;
const int OUTLET_PIN[] = { 16, 17, 18, 19, 21, 22, 23, 25 };
unsigned int outlet_state[N_OUTLETS];

void ivy_begin(){
  for(int outlet = 0; outlet < N_OUTLETS; outlet++){
    pinMode(OUTLET_PIN[outlet], OUTPUT);
    digitalWrite(OUTLET_PIN[outlet], 1);
    outlet_state[outlet] = 0;
  }

  dht.begin();
}

void ivy_setOutlet(unsigned int outlet, unsigned int state){
  if(outlet >= N_OUTLETS) return;
  outlet_state[outlet] = state;
  digitalWrite(OUTLET_PIN[outlet], (state != 0) ? 0 : 1);
}

unsigned int ivy_getOutlet(unsigned int outlet){
  if(outlet >= N_OUTLETS) return 0;
  return outlet_state[outlet];
}

float ivy_getHumidity(){
  if(millis() - last_dht_read >= 10000) { ivy_humidity = dht.readHumidity(); last_dht_read = millis();}
  if (isnan(ivy_humidity)) return -1;
  return ivy_humidity;
}

float ivy_getTemperature(){
  if(millis() - last_dht_read >= 10000) { ivy_temperature = dht.readTemperature(); last_dht_read = millis();}
  if (isnan(ivy_temperature)) return -1;
  return ivy_temperature;
}