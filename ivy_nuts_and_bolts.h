#ifndef IVY_NUTS_AND_BOLTS_H
#define IVY_NUTS_AND_BOLTS_H

/*
  Nuts and Bolts es una simple (y sucia) libreria
  que permite que varias otras librerias accedan a variables globales compartidas que son definidas acá

  Cada librería que necesite acceso a una variable compartida deberá declararla acá e incluir "ivy_nuts_and_bolts.h"
*/

struct Sensors{
  float temperature;
  float humidity;
} sensors;

struct tm timeinfo;
#endif