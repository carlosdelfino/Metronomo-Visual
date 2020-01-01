#ifndef PARAMETROS_H
#define PARAMETROS_H

// ativa depuração via Serial, o uso da serial pode causar assincronia dos pulsos do metromo
#define SHOW_SERIAL   false
// informa se o display deve ficar piscando
#define BLINK_LCD     false
// Seleciona qual algoritmo é o padrão
#define STATE_DEFAULT   1

#define POT_VCC       A1
#define POT_GND       A3
#define POT_DERIVADA  A2

#define BUTTON_PORT   A0

#define BUTTON_LIMIAR_DIREITA   100
#define BUTTON_LIMIAR_CIMA      200
#define BUTTON_LIMIAR_BAIXO     400
#define BUTTON_LIMIAR_ESQUERDA  600
#define BUTTON_LIMIAR_SELECIONA 800

// BPM máximo e mínimo
#define BPM_MAX      360
#define BPM_MED      120
#define BPM_MIN      30

// NOMES DOS BOTÕES
#define BAIXO         1
#define CIMA          2
#define ESQUERDA      3
#define DIREITA       4
#define SELECIONA     5

#define LCD_RS        8
#define LCD_ENABLE    9
#define LCD_D4        4
#define LCD_D5        5
#define LCD_D6        6
#define LCD_D7        7


double bpm = BPM_MIN;
double bpmShow;

const double TIME_BLINK = 700;
double last_blink = 0;

const double TIME_BUTTON = 350;
double last_cima = 0;
double last_baixo = 0;

byte selecionaState = STATE_DEFAULT;
byte stateShow = 99;
double last_seleciona = 0;

// tempo/stado do LED
byte ledState = 0;

// tempo da ultima mudança de estado dos leds
double last_led;

// matrix de LED a ser acionada confrome matrix de algoritimo
const byte LED[] = {13, 12, 11, 3};
#define LED_NUMBER  (sizeof LED / sizeof(byte))

#endif
