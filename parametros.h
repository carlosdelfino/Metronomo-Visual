/**
   Este arquivo possui todos os parametros gerais da aplicação.
   Ativação e Desativação da depuração
   Portas e Pinos a serem usados
   Parametros em geral.

   Display:
   The circuit:
   LCD RS pin to digital pin 13
   LCD Enable pin to digital pin 12
   LCD D4 pin to digital pin 4
   LCD D5 pin to digital pin 5
   LCD D6 pin to digital pin 6
   LCD D7 pin to digital pin 7
   LCD R/W pin to ground
   10K resistor:
   ends to +5V and ground

   Potenciometro
      A1 como vcc
      A2 como derivada
      A3 como gnd

   @Author: Carlos Delfino - consultoria@carlosdelfino.eti.br
   @Proponente: Severiano Mesquita - Severo
*/
#ifndef PARAMETROS_H
#define PARAMETROS_H

#include <Arduino.h>

#define VERSION "V1.5"

// ativa depuração via Serial, o uso da serial pode causar assincronia dos pulsos do metromo
#define SHOW_SERIAL   true

// informa se o display deve ficar piscando
#define BLINK_LCD     false
// Seleciona qual algoritmo é o padrão
#define ALGO_DEFAULT   1

#define POT_VCC       A3
#define POT_GND       A1
#define POT_DERIVADA  A2
// permite a leitura N vezes e faz a média para evitar instabilidades.
#define POT_DERIVADA_MEDIA 10

#define BUTTON_PORT   A0

#define BUTTON_LIMIAR_DIREITA   100
#define BUTTON_LIMIAR_CIMA      200
#define BUTTON_LIMIAR_BAIXO     400
#define BUTTON_LIMIAR_ESQUERDA  600
#define BUTTON_LIMIAR_SELECIONA 800

// NOMES DOS BOTÕES
#define BUTTON_DOWN   1
#define BUTTON_UP     2
#define BUTTON_SELECT 3
#define BUTTON_RIGHT  4
#define BUTTON_LEFT   5
#define BUTTON_NULL   99
// tempo que o botão deve ficar precionado para chamar segunda ação
#define BUTTON_TIME 2000


#define LCD_RS        8
#define LCD_ENABLE    9
#define LCD_D4        4
#define LCD_D5        5
#define LCD_D6        6
#define LCD_D7        7

#define LCD_BPM_TEXT       "BPM:"
#define LCD_BPM_TEXT_COL   0
#define LCD_BPM_TEXT_LINE  1
#define LCD_BPM_VALUE_COL  5 + LCD_BPM_TEXT_COL + 2
#define LCD_BPM_VALUE_LINE 1

#define LCD_BPM_BEAT_TEXT       "Beat:"
#define LCD_BPM_BEAT_TEXT_COL   9
#define LCD_BPM_BEAT_TEXT_LINE  0
#define LCD_BPM_BEAT_VALUE_COL  5 + LCD_BPM_BEAT_TEXT_COL + 1
#define LCD_BPM_BEAT_VALUE_LINE 0


#define LCD_STATE_TEXT       "ST:"
#define LCD_STATE_TEXT_COL   11
#define LCD_STATE_TEXT_LINE  1
#define LCD_STATE_VALUE_COL  3 + LCD_STATE_TEXT_COL + 1
#define LCD_STATE_VALUE_LINE 1

#define LCD_MEMORY_STATE_LINE 0
#define LCD_MEMORY_STATE_COL  0

// BPM máximo e mínimo
#define BPM_MAX      360
#define BPM_MAX_POT (BPM_MAX+2)
#define BPM_MED      120
#define BPM_MIN      30

// SUGESTÕES DE BPM PADRÕES PARA PROGRAMAÇÃO DA MEMÓRIA
#define BPM_PROG_DEFAULT_0 60
#define BPM_PROG_DEFAULT_1 80
#define BPM_PROG_DEFAULT_2 100
#define BPM_PROG_DEFAULT_3 120
#define BPM_DEFAULT BPM_PROG_DEFAULT_0

#define BPM_PROG_0 0
#define BPM_PROG_1 1
#define BPM_PROG_2 2
#define BPM_PROG_3 3
#define BPM_PROG_UP    BPM_PROG_2
#define BPM_PROG_DOWN  BPM_PROG_3
#define BPM_PROG_LEFT  BPM_PROG_0
#define BPM_PROG_RIGHT BPM_PROG_1
#define BPM_POT_LAST_CHANGE_TIME 100

#define TIME_BLINK 700


// matrix de LED a ser acionada conforme matrix de algoritimo
// 13, 12, 11, 03, 02, 01, 00 podem ser usadas com o shield
// 11 e 03 são PWM
// 0 e 1 são RX e TX
#define LED1 13
#define LED2 12
#define LED3 11
#define LED4 02

const byte LED[] = {LED1,LED2,LED3,LED4};
#define LED_NUMBER  (sizeof LED / sizeof(byte))

#define SENSOR_POTENCIOMETER 1
#define SENSOR_BUTTONS       2
#define SENSOR_DEFAULT SENSOR_POTENCIOMETER

#define SENSOR_TYPE_SHOW_TIME 300

#define WRITE_MEMORY_CHANGED_MAX_TIME 5000

// tempo entre cada adição do # na inicialização
// deve ser influenciada por sensor_type_show_time
#define SHOW_INIT_STEP_TIME 80

#define SHOW_BLINK_DELAY_STARTUP 300

#define SHOW_RESETMEMORY_DELAY_STARTUP 200
#endif
