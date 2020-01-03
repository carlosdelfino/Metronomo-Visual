/**
 * Neste arquivo são registrados parametros especificos
 * que interferem no comportamento do algoritmo trazendo 
 * novas abordagens para funcionamento do Metronomo.
 * 
 * @Author: Carlos Delfino - consultoria@carlosdelfino.eti.br
 * @Proponente: Severiano Mesquita - Severo
 */
#ifndef ALGORITIMO_H
#define ALGORITIMO_H

#include <Arduino.h>
#include "parametros.h"

// matrix de alogritmo que define a sequência de acendimento de LEDs
const boolean matrixAlgoLED[2][16][LED_NUMBER] =  {{
    {HIGH, LOW , LOW , LOW },
    {HIGH, LOW , LOW , LOW },
    {HIGH, LOW , LOW , LOW },
    {HIGH, LOW , LOW , LOW },
    {HIGH, HIGH, LOW , LOW },
    {HIGH, HIGH, LOW , LOW },
    {HIGH, HIGH, LOW , LOW },
    {HIGH, HIGH, LOW , LOW },
    {HIGH, HIGH, HIGH, LOW },
    {HIGH, HIGH, HIGH, LOW },
    {HIGH, HIGH, HIGH, LOW },
    {HIGH, HIGH, HIGH, LOW },
    {HIGH, HIGH, HIGH, HIGH},
    {HIGH, HIGH, HIGH, HIGH},
    {HIGH, HIGH, HIGH, HIGH},
    {LOW , LOW , LOW , LOW }
  }, {
    {HIGH, LOW , LOW , LOW },
    {HIGH, LOW , LOW , LOW },
    {HIGH, LOW , LOW , LOW },
    {HIGH, LOW , LOW , LOW },
    {LOW , HIGH, LOW , LOW },
    {LOW , HIGH, LOW , LOW },
    {LOW , HIGH, LOW , LOW },
    {LOW , HIGH, LOW , LOW },
    {LOW , LOW , HIGH, LOW },
    {LOW , LOW , HIGH, LOW },
    {LOW , LOW , HIGH, LOW },
    {LOW , LOW , HIGH, LOW },
    {HIGH, HIGH, HIGH, HIGH},
    {HIGH, HIGH, HIGH, HIGH},
    {HIGH, HIGH, HIGH, HIGH},
    {LOW , LOW , LOW , LOW }
  }
};

const double  matrixAlgoFator[2]               =  {4, 4};
#define LED_ALGO   (sizeof matrixAlgoLED       / sizeof matrixAlgoLED[0])
#define LED_STATES (sizeof matrixAlgoLED[0]    / sizeof matrixAlgoLED[0][0]) 


#endif
