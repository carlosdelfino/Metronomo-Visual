#ifndef ALGORITIMO_H
#define ALGORITIMO_H

#include "parametros.h"

// matrix de alogritmo que define a sequência de acendimento de LEDs
const double matrixAlgoFator[2]                =  {3, 3};
const boolean matrixAlgoLED[2][12][LED_NUMBER] =  {{
    {HIGH, LOW , LOW , LOW },
    {HIGH, LOW , LOW , LOW },
    {HIGH, LOW , LOW , LOW },
    {HIGH, HIGH, LOW , LOW },
    {HIGH, HIGH, LOW , LOW },
    {HIGH, HIGH, LOW , LOW },
    {HIGH, HIGH, HIGH, LOW },
    {HIGH, HIGH, HIGH, LOW },
    {HIGH, HIGH, HIGH, LOW },
    {HIGH, HIGH, HIGH, HIGH},
    {HIGH, HIGH, HIGH, HIGH},
    {LOW , LOW , LOW , LOW }
  }, {
    {HIGH, LOW , LOW , LOW },
    {HIGH, LOW , LOW , LOW },
    {HIGH, LOW , LOW , LOW },
    {LOW , HIGH, LOW , LOW },
    {LOW , HIGH, LOW , LOW },
    {LOW , HIGH, LOW , LOW },
    {LOW , LOW , HIGH, LOW },
    {LOW , LOW , HIGH, LOW },
    {LOW , LOW , HIGH, LOW },
    {HIGH, HIGH, HIGH, HIGH},
    {HIGH, HIGH, HIGH, HIGH},
    {LOW , LOW , LOW , LOW }
  }
};
#define LED_ALGO   (sizeof matrixAlgoLED       / sizeof matrixAlgoLED[0])
#define LED_STATES (sizeof matrixAlgoLED[0]    / sizeof matrixAlgoLED[0][0]) - 1

#endif
