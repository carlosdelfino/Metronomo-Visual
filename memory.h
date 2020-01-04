/**
   Arquivo com detalhes sobre programação dos parametros na EEPROM

   neste aquivo são declados as estruturas e assinaturas de funções
   @Author: Carlos Delfino - consultoria@carlosdelfino.eti.br
   @Proponente: Severiano Mesquita - Severo
*/
#ifndef MEMORY_H
#define MEMORY_H

#include <Arduino.h>
#include "parametros.h"

#ifndef BPM_PROG_SIZE
#define BPM_PROG_SIZE 4
#endif

#ifndef BPM_EEPROM_ADDRESS
#define BPM_EEPROM_ADDRESS 0
#endif

struct memory_t {
  int sensor;
  int algo;
  int bpm;
  int bpmProg[BPM_PROG_SIZE];
};

#endif
