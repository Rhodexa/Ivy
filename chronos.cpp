#include "chronos.h"

void Chronos::update(){
  if(counter) counter--;
}

void Chronos::iniciar(unsigned int segundos){
  counter = segundos;
}

bool Chronos::finalizado(){
  return counter == 0;
}

unsigned int Chronos::horasASegundos(unsigned int horas){
  return horas * 3600;
}

unsigned int Chronos::minutosASegundos(unsigned int minutos){
  return minutos * 60;
}