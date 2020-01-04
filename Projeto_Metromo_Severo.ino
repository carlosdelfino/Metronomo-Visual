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
#include <limits.h>

#include "parametros.h"
#include "algoritimos.h"
#include "memory.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

byte selecionaAlgo = STATE_DEFAULT;

/**
   Memória onde ficam dados relativos ao setup do firmware;
   Ela deve ser povoada com a leitura do default na EEPROM,
   se ela não estiver pronta deverá ser povoada com dados padrões
   e então gravada para proxima leitura.
*/
memory_t memory;
// joga para o máximo possível para que só tente gravar
// se o tempo da ultima mudança for menor que o tempo atual
double memoryChanged = __DBL_MAX__;


void setup() {

#if SHOW_SERIAL
  Serial.begin(9600);
  Serial.println(LED_ALGO);
  Serial.println(LED_STATES);
  Serial.print("Metrônomo Servero! - ");
  Serial.println(VERSION);
  Serial.println("Arduino Minas");
#endif

  readmemory();
  /*
     inicia a verificar se algum botão está precionado durante o setup
     veja detalhes na próxima chamdaa desta função
  */
  checkButtonSetup();

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

  /*
     Caso o botão ainda esteja precionado depois deste estágio,
     após a exibição inicial do jogo da velha. Assim o modo de
     leitura do bpm muda de botão para potenciometro
  */
  checkButtonSetup();

#if SHOW_SERIAL
  Serial.print("Sensor Type: ");
  Serial.println(sensorType);
#endif

  checkSetupMemoryData();

  // ###################################
  // não faça mais alterações de parametros no setup apartir daqui
  // checkMemoryData deve ser chamado por ultimo após todos os ajustes.

  // Print a message to the LCD.
  lcd.print("Metrônomo!");
  lcd.setCursor(4, 1);
  lcd.print("Severo! ");
  lcd.print(VERSION);
  for (int i = 0; i < 9; i++) {
    blinkLCD();
    delay(400);
  }

  lcdShowParameterTitle();

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

void lcdShowParameterTitle() {
  lcd.clear();
  lcd.setCursor(LCD_BPM_BEAT_TEXT_COL, LCD_BPM_BEAT_TEXT_LINE);
  lcd.print(LCD_BPM_BEAT_TEXT);

  lcd.setCursor(LCD_BPM_TEXT_COL, LCD_BPM_TEXT_LINE);
  lcd.print(LCD_BPM_TEXT);

  lcd.setCursor(LCD_STATE_TEXT_COL, LCD_STATE_TEXT_LINE);
  lcd.print(LCD_STATE_TEXT);
}

/**
   Verifica se a memória eeprom está com pelo meno os dados default;
*/
void checkSetupMemoryData() {
#if SHOW_SERIAL
  Serial.println("Verificando dados obtidos na memória e configurações default");
  serialShowMemoryDebug(memory);
#endif
  if (memory.bpm == -1
      || memory.sensor == -1
      || memory.algo == -1
     ) {
#if SHOW_SERIAL
    Serial.println("memoria zerada, gravando pela primeira vez!");
#endif
    memory.bpm = bpm;
    memory.sensor = sensorType;
    memory.algo = selecionaAlgo;
    memory.bpmProg[BPM_PROG_0] = BPM_PROG_DEFAULT_0;
    memory.bpmProg[BPM_PROG_1] = BPM_PROG_DEFAULT_1;
    memory.bpmProg[BPM_PROG_2] = BPM_PROG_DEFAULT_2;
    memory.bpmProg[BPM_PROG_3] = BPM_PROG_DEFAULT_3;
    writeMemory();
  } else {
    bpm = memory.bpm;
    sensorType = memory.sensor;
    selecionaAlgo = memory.algo;
  }

#if SHOW_SERIAL
  Serial.println("dados obtidos na memória e configurações ajustadas e checadas");
#endif
}

void checkButtonSetup() {
  static boolean selecionaSetup = false;
  byte button = readButton();
  switch (button) {
    case BUTTON_SELECT:
      if (selecionaSetup) changeSensor();
      else                selecionaSetup = true;
      break;
  }
}

/**
   Altera o funcionamento do sensor do BPM

   O Sensor de BPM é a leitura dos botões ou potenciometro para mudar o BPM.
   Caso seja usado botões, ficam apenas 2 memórias já que só sobram dois botões.
   Se mudar para potenciometro é possível usar 4 memórias.

   Isso não muda nada no funcionamento da estrutura que armazena os dados.
*/
void changeSensor() {
  if (sensorType == SENSOR_POTENCIOMETER) sensorType = SENSOR_BUTTONS;
  else  sensorType = SENSOR_POTENCIOMETER;

  writeMemory();
}

void loop() {
#if BLINK_LCD
  blinkLCD();
#endif

  checkButton();
  checkPotenciometer();

  showBPM();
  showState();
  showMemoryState();
  blinkLEDBPM();

  checkMemoryData();
}

void checkPotenciometer() {
  static double lastChangeBPM = millis();
  static  int newBPM, lastBPM;
  int analogicBPM;
  if ( sensorType == SENSOR_POTENCIOMETER && millis() - lastChangeBPM > BPM_POT_LAST_CHANGE_TIME) {
    analogicBPM = analogRead(POT_DERIVADA);
    analogicBPM += analogRead(POT_DERIVADA);
    analogicBPM += analogRead(POT_DERIVADA);
    analogicBPM += analogRead(POT_DERIVADA);
    analogicBPM /= 4;

    int    newBPM = map(analogicBPM, 0, 1023, BPM_MIN, BPM_MAX_POT);
    // é necessário verificar se o BPM mudou para setar a variável
    // assim evita-se remover o bpm obtido da memória;
    if (newBPM != lastBPM) {
      bpm = newBPM;
      lastBPM = newBPM;
    }

    // Quando a mudança de bpm é pelo potencimetro analógico não precisa gravar na memória
    // ele grava pela posição mecânica do potenciometro.
    // é preciso analisar soluções com potenciometros digitais/tipo encoder
    lastChangeBPM = millis();
  }
#if SHOW_SERIAL
  Serial.print("Analogic BPM: ");
  Serial.print(analogicBPM);
  Serial.print(" BPM: ");
  Serial.println(bpm);
#endif

}

void showMemoryState() {
  static String showMemoryState = "#";
  lcd.setCursor(LCD_MEMORY_STATE_COL, LCD_MEMORY_STATE_LINE);
  if (memoryChanged != __DBL_MAX__) {
    showMemoryState = (showMemoryState == "#") ? "*" : "#";
    lcd.print(showMemoryState);
  } else
    lcd.print(" ");
}

void checkMemoryData() {
  if (millis() - memoryChanged > WRITE_MEMORY_CHANGED_MAX_TIME) {
    writeMemory();
    // joga para o máximo possível para que só tente gravar
    // se o tempo da ultima mudança for menor que o tempo atual
    memoryChanged = __DBL_MAX__;
  }
}

void showState() {
  static byte stateShow = 99;
  if (stateShow != selecionaAlgo) {
    lcd.setCursor(LCD_STATE_VALUE_COL, LCD_STATE_VALUE_LINE);
    //    lcd.print("  ");
    //    lcd.setCursor(14, 1);
    lcd.print(selecionaAlgo);
    stateShow = selecionaAlgo;
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
  if (millis() - last_led >= bpmCalc / matrixAlgoFator[selecionaAlgo]) {
#if SHOW_SERIAL
    Serial.print("Algo: ");
    Serial.print(selecionaAlgo);
    Serial.print(" ");
#endif
    for (byte l = 0; l < LED_NUMBER; l++) {
      digitalWrite(LED[l], matrixAlgoLED[selecionaAlgo][ledState][l]);
#if SHOW_SERIAL
      Serial.print(matrixAlgoLED[selecionaAlgo][ledState][l]);
      Serial.print(" ");
#endif
    }
    showBPMTime((ledState / matrixAlgoFator[selecionaAlgo]) + 1);

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
  static double lastUp = millis();
  static double lastDown = millis();
  static double lastLeft = millis();
  static double lastRight = millis();
  static double lastSelect = millis();

  byte button = readButton();
  switch (button) {
    case BUTTON_RIGHT:
      if (millis() - lastLeft >= TIME_BUTTON) {
        readBPMProg(BPM_PROG_RIGHT);
        lastRight = millis();
      }
      break;
    case BUTTON_LEFT:
      if (millis() - lastLeft >= TIME_BUTTON) {
        readBPMProg(BPM_PROG_LEFT);
        lastLeft = millis();
      }
      break;
    case BUTTON_UP:
      if (millis() - lastUp >= TIME_BUTTON) {
        if (sensorType == SENSOR_BUTTONS) upBPM();
        else readBPMProg(BPM_PROG_UP);
        lastUp = millis();
      }
      break;
    case BUTTON_DOWN:
      if (millis() - lastDown >= TIME_BUTTON) {
        if (sensorType == SENSOR_BUTTONS) downBPM();
        else readBPMProg(BPM_PROG_DOWN);
        lastDown = millis();
      }
      break;
    case BUTTON_SELECT:
      if (millis() - lastSelect >= TIME_BUTTON) {
        selecionaAlgo = ++selecionaAlgo >= LED_ALGO ? 0 : selecionaAlgo;
        memory.algo = selecionaAlgo;
        memoryChanged = millis();
        lastSelect = millis();
      }
      break;
  }
}

void readBPMProg(byte prog) {
  bpm = memory.bpmProg[prog];
}

void upBPM() {
  bpm++;
  bpm = min(BPM_MAX, bpm);
  memory.bpm = bpm;
  memoryChanged = millis();
}

void downBPM() {
  bpm--;
  bpm = max(BPM_MIN, bpm);
  memory.bpm = bpm;
  memoryChanged = millis();
}
byte readButton() {
  int  botao = analogRead (BUTTON_PORT);
  if (botao < BUTTON_LIMIAR_DIREITA) {
    return BUTTON_RIGHT;
  }   else if (botao < BUTTON_LIMIAR_CIMA) {
    return BUTTON_UP;
  }   else if (botao < BUTTON_LIMIAR_BAIXO) {
    return BUTTON_DOWN;
  }   else if (botao < BUTTON_LIMIAR_ESQUERDA) {
    return BUTTON_LEFT;
  }   else if (botao < BUTTON_LIMIAR_SELECIONA) {
    return BUTTON_SELECT;
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


void writeMemory() {
#if SHOW_SERIAL
  Serial.print("Write Memoria: ");
  Serial.println(BPM_EEPROM_ADDRESS);
  serialShowMemoryDebug(memory);
  Serial.println();
  Serial.println();
#endif
  //    uint8_t* src = (uint8_t*)memory;
  //  uint8_t* dst = (uint8_t*)BPM_EEPROM_ADDRESS;
  //    for (uint16_t i = 0; i < len(memory_t); i++) {
  //        eeprom_write_byte(dst++, *src++);
  //    }
  EEPROM.put(BPM_EEPROM_ADDRESS, memory);

#if SHOW_SERIAL
  Serial.println("write Memoria - done");
  Serial.println();
#endif
}

void readmemory() {
#if SHOW_SERIAL
  Serial.print("Read Memoria: ");
  Serial.println(BPM_EEPROM_ADDRESS);
#endif

  EEPROM.get(BPM_EEPROM_ADDRESS, memory);

#if SHOW_SERIAL
  Serial.println("Read Memoria");
  serialShowMemoryDebug(memory);
  Serial.println();
  Serial.println("Read Memoria - done");
  Serial.println();
#endif
}

void serialShowMemoryDebug(struct memory_t mem) {
  Serial.print("BPM: ");
  Serial.print(mem.bpm);
  Serial.print("  BPM Programado: ");
  for (int i = 0; i < BPM_PROG_SIZE; i++) {
    Serial.print(mem.bpmProg[i]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.print("Sensor Type: ");
  Serial.println(mem.sensor);
  Serial.print("Algoritimo: ");
  Serial.println(mem.algo);

}
