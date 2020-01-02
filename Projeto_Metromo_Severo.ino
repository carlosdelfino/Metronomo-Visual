/**
   Metronomo Visual

   A proposta aprsentada por Severiano vem da necessidade
   um metronomo Visual que permita aos músicos sincronizarem
   as músicas

   @Author: Carlos Delfino - consultoria@carlosdelfino.eti.br
   @Proponente: Severiano Mesquita - Severo

*/

// include the library code:
#include <LiquidCrystal.h>
#include <EEPROM.h>

#include "parametros.h"
#include "algoritimos.h"
#include "memoria.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

byte selecionaState = STATE_DEFAULT;

void setup() {
#if SHOW_SERIAL
  Serial.begin(9600);
  Serial.println(LED_ALGO);
  Serial.println(LED_STATES);
  Serial.println("Metrônomo Servero! - V1.1");
  Serial.println("Arduino Minas");
#endif

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  for (int i = 0; i < 16; i++) {
    lcd.print("#");
    delay(80);
  }
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    lcd.print("#");
    delay(80);
  }
  lcd.clear();

  // Print a message to the LCD.
  lcd.print("Metrônomo!");
  lcd.setCursor(9, 1);
  lcd.print("Severo!");
  for (int i = 0; i < 9; i++) {
    blinkLCD();
    delay(400);
  }
  lcd.clear();

  lcd.setCursor(LCD_BPM_BEAT_TEXT_COL, LCD_BPM_BEAT_TEXT_LINE);
  lcd.print(LCD_BPM_BEAT_TEXT);

  lcd.setCursor(LCD_BPM_TEXT_COL, LCD_BPM_TEXT_LINE);
  lcd.print(LCD_BPM_TEXT);

  lcd.setCursor(LCD_STATE_TEXT_COL, LCD_STATE_TEXT_LINE);
  lcd.print(LCD_STATE_TEXT);

  pinMode(POT_VCC, OUTPUT);
  pinMode(POT_GND, OUTPUT);
  digitalWrite(POT_VCC, LOW);
  digitalWrite(POT_GND, HIGH);

  for (byte l = 0; l < LED_NUMBER; l++) {
    pinMode(LED[l], OUTPUT);
    digitalWrite(LED[l], LOW);
  }

  lcd.display();
}

void loop() {
#if BLINK_LCD
  blinkLCD();
#endif

  checkButton();

  showBPM();
  showState();

  blinkLEDBPM();
}

void showState() {
  static byte stateShow = 99;
  if (stateShow != selecionaState) {
    lcd.setCursor(LCD_STATE_VALUE_COL, LCD_STATE_VALUE_LINE);
//    lcd.print("  ");
//    lcd.setCursor(14, 1);
    lcd.print(selecionaState);
    stateShow = selecionaState;
  }

}
void showBPM() {
  static double bpmShow;
  if (bpmShow != bpm) {
    lcd.setCursor(5, 1);
    lcd.print("    ");
    lcd.setCursor(5, 1);
    lcd.print((int)bpm);
    bpmShow = bpm;
  }
}

void blinkLEDBPM() {
  static double last_led;
  static byte ledState = 0;

  double bpmCalc = (1 / (bpm / 60) * 1000);
  if (millis() - last_led >= bpmCalc / matrixAlgoFator[selecionaState]) {
#if SHOW_SERIAL
    Serial.print("Algo: ");
    Serial.print(selecionaState);
    Serial.print(" ");
#endif
    for (byte l = 0; l < LED_NUMBER; l++) {
      digitalWrite(LED[l], matrixAlgoLED[selecionaState][ledState][l]);
#if SHOW_SERIAL
      Serial.print(matrixAlgoLED[selecionaState][ledState][l]);
      Serial.print(" ");
#endif
    }
    showBPMTime((ledState / matrixAlgoFator[selecionaState]) + 1);

#if SHOW_SERIAL
    Serial.println();
#endif
    ledState = (++ledState >= LED_STATES) ? 0 : ledState;
    last_led = millis();
  }
}

void showBPMTime(byte l) {
  static byte lastLed;
  if (lastLed != l) {
    lcd.setCursor(LCD_BPM_BEAT_VALUE_COL, LCD_BPM_BEAT_VALUE_LINE);
    lcd.print(l);
    lastLed = l;
  }
}

void checkButton() {
  static double last_cima = millis();
  static double last_baixo = millis();
  static double last_seleciona = millis();

  byte button = readButton();
  switch (button) {
    case CIMA:
      if (millis() - last_cima >= TIME_BUTTON) {
        bpm++;
        bpm = min(BPM_MAX, bpm);
        last_cima = millis();
      }
      break;
    case BAIXO:
      if (millis() - last_baixo >= TIME_BUTTON) {
        bpm--;
        bpm = max(BPM_MIN, bpm);
        last_baixo = millis();
      }
      break;
    case SELECIONA:
      if (millis() - last_seleciona >= TIME_BUTTON) {
        selecionaState = ++selecionaState >= LED_ALGO ? 0 : selecionaState;
        last_seleciona = millis();
      }
      break;
  }
}
byte readButton() {
  int  botao = analogRead (BUTTON_PORT);
  if (botao < BUTTON_LIMIAR_DIREITA) {
    return DIREITA;
  }   else if (botao < BUTTON_LIMIAR_CIMA) {
    return CIMA;
  }   else if (botao < BUTTON_LIMIAR_BAIXO) {
    return BAIXO;
  }   else if (botao < BUTTON_LIMIAR_ESQUERDA) {
    return ESQUERDA;
  }   else if (botao < BUTTON_LIMIAR_SELECIONA) {
    return SELECIONA;
  }
  return NULL;
}

void blinkLCD() {
  static boolean lcd_display = false;
  static double last_blink = millis();

  if (millis() - last_blink >= TIME_BLINK) {
    if (lcd_display) {
      lcd.noDisplay();
      lcd_display = false;
    } else {
      lcd.display();
      lcd_display = true;
    }
    last_blink = millis();
  }
}


void writeMemoria(struct memoria_t input) {
  //    uint8_t* src = (uint8_t*)input;
  //  uint8_t* dst = (uint8_t*)BPM_EEPROM_ADDRESS;
  //    for (uint16_t i = 0; i < len(memoria_t); i++) {
  //        eeprom_write_byte(dst++, *src++);
  //    }
  EEPROM.get(BPM_EEPROM_ADDRESS, input);

#if SHOW_SERIAL
  Serial.println("Write Memoria");
  Serial.print("Default BPM: ");
  Serial.print(input.bpm);
  Serial.print("  BPM Programado: ");
  for (int i = 0; i < BPM_PROG_SIZE; i++) {
    Serial.print(input.bpmProg[i]);
    Serial.print(" ");
  }
  Serial.println();
#endif
}

void readMemoria(struct memoria_t output) {
  //  uint8_t* src = (uint8_t*)BPM_EEPROM_ADDRESS;
  //   uint8_t* dst = (uint8_t*)output;
  //   for (uint16_t i = 0; i < len(memoria_t); i++) {
  //       *dst++ = eeprom_read_byte(src++);
  //   }

  EEPROM.get(BPM_EEPROM_ADDRESS, output);

#if SHOW_SERIAL
  Serial.println("Write Memoria");
  Serial.print("Default BPM: ");
  Serial.print(&output.bpm);
  Serial.print("  BPM Programado: ");
  for (int i = 0; i < BPM_PROG_SIZE; i++) {
    Serial.print(&output.bpmProg[i]);
    Serial.print(" ");
  }
  Serial.println();
#endif
}
