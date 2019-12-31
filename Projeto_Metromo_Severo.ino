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

#define BLINK_LCD false

// BPM máximo e mínimo
#define BPM_MAX 360
#define BPM_MIN 30

// NOMES DOS BOTÕES
#define BAIXO 1
#define CIMA 2
#define ESQUERDA 3
#define DIREITA 4
#define SELECTE 5

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int bpm = 30;

const double time_blink = 700;
double last_blink = 0;

double last_led = 0;

const double TIME_BUTTON = 250;
double last_cima = 0;
double last_baixo = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Metromo Severo!");

  lcd.setCursor(0, 1);
  lcd.print("BPM :");


  pinMode(A1, OUTPUT);
  pinMode(A3, OUTPUT);
  digitalWrite(A1, LOW);
  digitalWrite(A3, HIGH);

  pinMode(13, OUTPUT);
  pinMode(11, OUTPUT);

  digitalWrite(13, LOW);
  digitalWrite(11, LOW);

}

void loop() {
  analogWrite(11, map(analogRead(A2), 0, 1024, 0, 255) );

if(BLINK_LCD)  blinkLCD();

  lcd.setCursor(5, 1);
  lcd.print("             ");
  lcd.setCursor(5, 1);

  byte button = readButton();

  checaBPM(button);

  lcd.print(bpm);

lcd.print("##");
  lcd.print(analogRead(A0));

  piscaTodos();
}

void piscaTodos() {
  if (millis() - last_led >= bpm * 60 * 1000 / 2) {
    digitalWrite(13, !digitalRead(13));
    last_led = millis();
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
  int  botao = analogRead (A0);  //Leitura do valor da porta analógica A0
  if (botao < 100) {
    return DIREITA;
  }   else if (botao < 200) {
    return CIMA;
  }   else if (botao < 400) {
    return BAIXO;
  }   else if (botao < 600) {
    return ESQUERDA;
  }   else if (botao < 800) {
    return SELECTE;
  }
  return NULL;
}

void blinkLCD() {
  static boolean lcd_display = false;
  if (millis() - last_blink >= time_blink) {
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

