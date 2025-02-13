#include <WebServer.h>
#include "ivy_wifi.h"
#include "ivy_nuts_and_bolts.h"
#include "ivy_relay.h"
#include "DHT.h"
#include "ArduinoOTA.h"
#include "time.h"
#include "time_tools.h"

DHT dht(4, DHT11);

const int GMT_MINUS_3 = -3600*3; // GMT-3

auto current_millis = millis();

auto last_sensor_update = current_millis;
void updateSensors(){
  if(current_millis - last_sensor_update >= 10000){
    last_sensor_update = current_millis;
    getLocalTime(&timeinfo); // I also update the time here!
    sensors.humidity = dht.readHumidity();
    sensors.temperature = dht.readTemperature();

    Serial.print(timeinfo.tm_hour);
    Serial.print(":");
    Serial.print(timeinfo.tm_min);
    Serial.print(":");
    Serial.print(timeinfo.tm_sec);
    Serial.print(" - ");
    Serial.print("Humidity: ");
    Serial.print(sensors.humidity);
    Serial.print(" Temp: ");
    Serial.print(sensors.temperature);
    Serial.println("");
    
    Serial.println('\n');
  } 
}

void setup() {
  Serial.begin(115200);
  ivy_wifi_begin();
  dht.begin();
  ivy_relay_begin();
  ArduinoOTA.begin();
  ArduinoOTA.setPassword("1234");
  configTime(GMT_MINUS_3, 0, "pool.ntp.org");
}

auto last_logic_run = current_millis;
void loop() {
  ArduinoOTA.handle();
  ivy_wifi_loop();
  updateSensors();
  current_millis = millis();
  if(current_millis - last_logic_run >= 1000){
    last_logic_run = current_millis;
    onLogic();
    ivy_updateOutlets();
  }
}

// Aliases y nicknames para adaptar los idiomas!
const int ENCENDIDO = ON;
const int APAGADO = OFF;
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

#define LUCES_1 TOMA_0
#define VENTILADOR TOMA_1
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Vos te encargás de la logica, yo me encargo del resto

void onLogic(){
  // Logica de la Humedad
  /*
  if(HUMEDAD < 40) { TOMA_0 = ENCENDIDO; } // Minima
  if(HUMEDAD > 60) { TOMA_0 = APAGADO; }   // Maxima
  */
  if(timeinfo.tm_min == 0)   { VENTILADOR = ENCENDIDO; }
  if(timeinfo.tm_min == 30)  { VENTILADOR = APAGADO; }

  if( entre_horas(20, 11) ) {
    LUCES_1 = ENCENDIDO;
  }
  else {
    LUCES_1 = APAGADO;
  }
}








