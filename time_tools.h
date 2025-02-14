#ifndef IVY_TIME_TOOLS_H
#define IVY_TIME_TOOLS_H
#include "ivy_nuts_and_bolts.h"

/*
  Time-Tools es una colección de herramientas que te permiten procesar tiempo agilmente...
*/

/* Éstas funciones te dan información sobre la fecha y hora */
int hora(){ return timeinfo.tm_hour; }
int minuto(){ return timeinfo.tm_min; }
int segundo(){ return timeinfo.tm_sec; }
int dia(){ return timeinfo.tm_dia; }

/* entre_horas(a, b) checkea que la hora actual (sin minutos ni segundos) esté entre las horas a y b, devuelve 'SI' cuando la condición se cumple y 'NO' en otros casos
   Ej:
  entre_horas(20, 11) checkea si son entre las 20 (8 PM) y las 11 (11 AM),
  si son las 12 (12 PM), esto devuelve 'NO', porque 12 (12PM) es despues de las 20 (8 PM) pero no es antes de las 11 (11 AM)
  pero si son las 00 (12 AM) entoces devuelve 'SI', porque 12 AM es despues de 8 PM pero antes de 11 AM
*/ 
bool entre_horas(int a, int b) {
  if (a <= b) return (a <= timeinfo.tm_hour) && (timeinfo.tm_hour < b);
  return (timeinfo.tm_hour >= a) || (timeinfo.tm_hour < b);
}




#endif