/**
   Arquivo com detalhes sobre programação dos parametros na EEPROM

   neste aquivo são declados as estruturas e assinaturas de funções
   @Author: Carlos Delfino - consultoria@carlosdelfino.eti.br
   @Proponente: Severiano Mesquita - Severo
*/
#ifndef MEMORIA_H
#define MEMORIA_H

#include <Arduino.h>
#include "parametros.h"

#ifndef BPM_PROG_SIZE
#define BPM_PROG_SIZE 4
#endif

#ifndef BPM_EEPROM_ADDRESS
#define BPM_EEPROM_ADDRESS 0
#endif

struct memoria_t {
  int bpm;
  int bpmProg[BPM_PROG_SIZE];
};

void readMemoria(struct memoria_t input) ;
void writeMemoria(struct memoria_t output) ;

#endif
