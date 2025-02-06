#include "ivy_wifi.h"
//#include "ivy_relay.h"

void setup() {
  Serial.begin(115200);
  ivy_wifi_begin();  
}

void loop() {
  ivy_wifi_loop();
}
