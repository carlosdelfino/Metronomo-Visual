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

#define BLINK_LCD    false

#define POT_VCC      A1
#define POT_GND      A3
#define POT_DERIVADA A2

#define BUTTON_PORT  A0

// BPM máximo e mínimo
#define BPM_MAX      360
#define BPM_MIN      30

#define LED1         13
#define LED2         12
#define LED3         11
#define LED4          3

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

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

double bpm = 30;
double bpmShow;

const double TIME_BLINK = 700;
double last_blink = 0;

double last_led1 = 0;
double last_led2 = 0;
double last_led3 = 0;
double last_led4 = 0;

const double TIME_BUTTON = 350;
double last_cima = 0;
double last_baixo = 0;

void setup() {
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

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);

}

void loop() {
  //  analogWrite(LED3, map(analogRead(POT_DERIVADA), 0, 1024, 0, 255) );

#if BLINK_LCD
  blinkLCD();
#endif

  byte button = readButton();

  checaBPM(button);

  showBPM();

  piscaTodos();
}

void showBPM(){
  if(bpmShow != bpm){
  lcd.setCursor(5, 1);
  lcd.print("    ");
  lcd.setCursor(5, 1);
  lcd.print(bpm);
  bpmShow = bpm;
  }

}

void piscaTodos() {
  static byte ledState = 1;
  double bpmCalc = (bpm / 60) * 1000;
      if (millis() - last_led4 >= bpmCalc / 2) {
        digitalWrite(LED4, digitalRead(LED1));
        last_led4 = millis();
      }
      if (millis() - last_led3 >= bpmCalc / 3) {
        digitalWrite(LED3, digitalRead(LED1));
        last_led3 = millis();
      }
      if (millis() - last_led2 >= bpmCalc / 4) {
        digitalWrite(LED2, digitalRead(LED1));
        last_led2 = millis();
      }
      if (millis() - last_led1 >= bpmCalc) {
        digitalWrite(LED1, !digitalRead(LED1));
        last_led1 = millis();
      }
}

void checaBPM(byte button) {
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
  }
}
byte readButton() {
  int  botao = analogRead (BUTTON_PORT);  //Leitura do valor da porta analógica A0
  if (botao < 100) {
    return DIREITA;
  }   else if (botao < 200) {
    return CIMA;
  }   else if (botao < 400) {
    return BAIXO;
  }   else if (botao < 600) {
    return ESQUERDA;
  }   else if (botao < 800) {
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

