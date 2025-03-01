#ifndef CHRONOS_H
#define CHRONOS_H

class Chronos {
public:
  unsigned int counter;

public:
  void update();
  void iniciar(unsigned int segundos);
  bool finalizado();
  unsigned int horasASegundos(unsigned int horas);
  unsigned int minutosASegundos(unsigned int minutos);
};

#endif