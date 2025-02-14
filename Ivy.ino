/*
              ////////////\  
              \___/// ____/ 
                 /// /      ///\    ///\  ///\    ///\
                /// /      /// /   /// / /// /   /// /
               /// /        /// /// __/   /// /// __/
          ////////////\      /// __/       /// __/
          \___________/      \__/       /// __/
        /////////////////////////////// ___/    ///\
        \______________________________/        \__/

      Made by Rhodune, with Absolute Love. Distributed under the Happy Bunny license
*/

#include <WebServer.h>
#include "ivy_wifi.h"
#include "ivy_nuts_and_bolts.h"
#include "ivy_relay.h"
#include "DHT.h"
#include "ArduinoOTA.h"
#include "time.h"
#include "time_tools.h"
DHT dht(4, DHT11);

const int BUTTON_PIN = 13
const int LED_PIN = 2

auto current_millis = millis();

auto last_sensor_update = current_millis;
void updateSensors(){
  if(current_millis - last_sensor_update >= 10000){
    last_sensor_update = current_millis;
    getLocalTime(&timeinfo); // I also update the time here!
    sensors.humidity = dht.readHumidity();
    sensors.temperature = dht.readTemperature();
  } 
}

bool booted_in_config_mode = false;

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  if(!digitalRead(BUTTON_PIN)) booted_in_config_mode = true;

  if(booted_in_config_mode){

  }
  else {
    ivy_wifi_begin();  
    dht.begin();
    ivy_relay_begin();
    ArduinoOTA.begin();
    ArduinoOTA.setPassword("1234");
    configTime(-10800, 0, "pool.ntp.org");
  }
}

auto last_logic_run = current_millis;

void loop() {
  ArduinoOTA.handle();
  current_millis = millis();
  if(booted_in_config_mode){

  }
  else{
    ivy_wifi_loop();
    updateSensors();
    if(current_millis - last_logic_run >= 1000){
      last_logic_run = current_millis;
      onLogic();
      ivy_updateOutlets();
    }
  }
}

// Aliases y nicknames para adaptar los idiomas!
const int SI = ON;
const int NO = OFF;
float& TEMPERATURA = sensors.temperature;
float& HUMEDAD = sensors.humidity;
unsigned int& TOMA_0 = outlet_state[0];
unsigned int& TOMA_1 = outlet_state[1];
unsigned int& TOMA_2 = outlet_state[2];
unsigned int& TOMA_3 = outlet_state[3];
unsigned int& TOMA_4 = outlet_state[4];
unsigned int& TOMA_5 = outlet_state[5];
unsigned int& TOMA_6 = outlet_state[6];
unsigned int& TOMA_7 = outlet_state[7];


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Vos te encargás de la logica, yo me encargo del resto
// ¡No uses delay()! Usá un timer basado en millis(), micros(), o el RTC (Hora, Minuto, Segundo, Dia, etc...) o incluso basado en formato UNIX si querés usar sólo segundos


// Acá podés redefinir el nombre de los tomacorrientes:
#define LUCES_1    TOMA_0
#define VENTILADOR TOMA_1

void onLogic(){
  if(minuto() == 0)   { VENTILADOR = SI; }
  if(minuto() == 30)  { VENTILADOR = NO; }

  // entre_horas(a, b) devuelve 'SI' si la hora actual está entre estas dos horas, y 'NO' cuando la hora actual no está entre esas dos horas
  // como LUCES_1 también usa SI y NO, se pueden asignar directamente.
  LUCES_1 = entre_horas(20, 11); 
  LUCES_2 = entre_horas(0, 12);
}








