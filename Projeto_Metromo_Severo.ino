/*
  LiquidCrystal Library - display() and noDisplay()

  Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
  library works with all LCD displays that are compatible with the
  Hitachi HD44780 driver. There are many of them out there, and you
  can usually tell them by the 16-pin interface.

  This sketch prints "Hello World!" to the LCD and uses the
  display() and noDisplay() functions to turn on and off
  the display.

  The circuit:
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

  Library originally added 18 Apr 2008
  by David A. Mellis
  library modified 5 Jul 2009
  by Limor Fried (http://www.ladyada.net)
  example added 9 Jul 2009
  by Tom Igoe
  modified 22 Nov 2010
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/LiquidCrystalDisplay

*/

#include "parametros.h"
#include "algoritimos.h"

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup() {
#if SHOW_SERIAL
  Serial.begin(9600);
  Serial.println(LED_ALGO);
  Serial.println(LED_STATES);
  Serial.println("Metrono Servero! - V1.1");
  Serial.println("Arduino Minas");
#endif

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Metromo Severo!");

  lcd.setCursor(0, 1);
  lcd.print("BPM :");

  pinMode(POT_VCC, OUTPUT);
  pinMode(POT_GND, OUTPUT);
  digitalWrite(POT_VCC, LOW);
  digitalWrite(POT_GND, HIGH);

  for (byte l = 0; l < LED_NUMBER; l++) {
    pinMode(LED[l], OUTPUT);
    digitalWrite(LED[l], LOW);
  }
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
  if (stateShow != selecionaState) {
    lcd.setCursor(14, 1);
    lcd.print("  ");
    lcd.setCursor(14, 1);
    lcd.print(selecionaState);
    stateShow = selecionaState;
  }

}
void showBPM() {
  if (bpmShow != bpm) {
    lcd.setCursor(5, 1);
    lcd.print("    ");
    lcd.setCursor(5, 1);
    lcd.print(bpm);
    bpmShow = bpm;
  }
}

void blinkLEDBPM() {
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
#if SHOW_SERIAL
    Serial.println();
#endif
    ledState = (++ledState == LED_STATES) ? 0 : ledState;
    last_led = millis();
  }
}

void checkButton() {
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

