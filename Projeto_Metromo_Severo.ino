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

// a função tem parametros default, portanto precisa ser sua assinatura declarada aqui
void showSensorType(boolean wait = true, long waitTime = SENSOR_TYPE_SHOW_TIME);
void showResetMemory(boolean wait = true, long waitTime = SHOW_RESETMEMORY_DELAY_STARTUP);
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

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

  readMemory();
  /*
     inicia a verificar se algum botão está precionado durante o setup
     veja detalhes na próxima chamdaa desta função
  */
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  checkButtonSetup();
  showSensorType(false);

  lcd.setCursor(0, 0);
  for (int i = 0; i < 16; i++) {
    lcd.print("#");
    delay(SHOW_INIT_STEP_TIME);
  }
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    lcd.print("#");
    delay(SHOW_INIT_STEP_TIME);
  }

  /*
     Caso o botão ainda esteja precionado depois deste estágio,
     após a exibição inicial do jogo da velha. Assim o modo de
     leitura do bpm muda de botão para potenciometro
  */
  checkButtonSetup();
  showSensorType(true);
  waitReleaseButtonSetup();

  lcd.clear();

#if SHOW_SERIAL
  Serial.print("Sensor Type: ");
  Serial.println(memory.sensor);
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
    delay(SHOW_BLINK_DELAY_STARTUP);
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

void showSensorType(boolean wait, long waitTime) {
  lcd.setCursor(0, 0);
  lcd.print("Sensor: ");
  if (memory.sensor == SENSOR_POTENCIOMETER) {
    lcd.print("POTENCIOMETER");
  }
  else {
    lcd.print("BUTTONS");
  }
  if (wait)  delay(waitTime); // dar tempo do usuário ler o tipo de sensor

}
/**
   Aguarda durante o setup que o usuário solte o botão Select
*/
void waitReleaseButtonSetup() {
  while (readButton() == BUTTON_SELECT) delay(100);
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
    resetMemory();
  }

#if SHOW_SERIAL
  Serial.println("dados obtidos na memória e configurações ajustadas e checadas");
#endif
}

void checkButtonSetup() {
  static byte selecionaSetup = BUTTON_NULL;
  byte button = readButton();
  switch (button) {
    case BUTTON_LEFT:
      if (selecionaSetup == BUTTON_LEFT) {
        resetMemory();
      } else {
        selecionaSetup = BUTTON_LEFT;
      }
      break;
    case BUTTON_SELECT:
      if (selecionaSetup == BUTTON_SELECT) {
        changeSensor();
#if SHOW_SERIAL
        Serial.println("Troca do Sensor finalizada!");
#endif
      }
      else {
#if SHOW_SERIAL
        Serial.println("Inicia Troca do Sensor: ");
#endif
        selecionaSetup = BUTTON_SELECT;
      }
      break;
  }
}

void resetMemory() {
  memory.bpm = BPM_DEFAULT;
  memory.sensor = SENSOR_DEFAULT;
  memory.algo = ALGO_DEFAULT;
  memory.bpmProg[BPM_PROG_0] = BPM_PROG_DEFAULT_0;
  memory.bpmProg[BPM_PROG_1] = BPM_PROG_DEFAULT_1;
  memory.bpmProg[BPM_PROG_2] = BPM_PROG_DEFAULT_2;
  memory.bpmProg[BPM_PROG_3] = BPM_PROG_DEFAULT_3;
  showResetMemory();
  writeMemory();
}

void showResetMemory(boolean wait,long waitDelay) {
  lcd.setCursor(0, 1);
  lcd.print("Memory Default!");
  if (wait)delay(waitDelay);
}
/**
   Altera o funcionamento do sensor do BPM

   O Sensor de BPM é a leitura dos botões ou potenciometro para mudar o BPM.
   Caso seja usado botões, ficam apenas 2 memórias já que só sobram dois botões.
   Se mudar para potenciometro é possível usar 4 memórias.

   Isso não muda nada no funcionamento da estrutura que armazena os dados.
*/
void changeSensor() {
#if SHOW_SERIAL
  Serial.print("Tipo de sensor era: ");
  Serial.print(memory.sensor);
#endif
  memory.sensor = (memory.sensor == SENSOR_POTENCIOMETER) ? SENSOR_BUTTONS : SENSOR_POTENCIOMETER;


#if SHOW_SERIAL
  Serial.print(", agora é:  ");
  Serial.println(memory.sensor);
#endif

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
  int analogicBPM = 0;
  if ( memory.sensor == SENSOR_POTENCIOMETER && millis() - lastChangeBPM > BPM_POT_LAST_CHANGE_TIME) {
    for (int i = 0 ; i < POT_DERIVADA_MEDIA; i++)
      analogicBPM += analogRead(POT_DERIVADA);
    analogicBPM /= POT_DERIVADA_MEDIA;

    int    newBPM = map(analogicBPM, 0, 1023, BPM_MIN, BPM_MAX_POT);
    // é necessário verificar se o BPM mudou para setar a variável
    // assim evita-se remover o bpm obtido da memória;
    if (newBPM != lastBPM) {
      memory.bpm = newBPM;
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
  Serial.println(memory.bpm);
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
  if (stateShow != memory.algo) {
    lcd.setCursor(LCD_STATE_VALUE_COL, LCD_STATE_VALUE_LINE);
    //    lcd.print("  ");
    //    lcd.setCursor(14, 1);
    lcd.print(memory.algo);
    stateShow = memory.algo;
  }

}
void showBPM() {
  static double bpmShow;
  if (bpmShow != memory.bpm) {
    lcd.setCursor(5, 1);
    lcd.print("    ");
    lcd.setCursor(5, 1);
    lcd.print((int)memory.bpm);
    bpmShow = memory.bpm;
  }
}

void blinkLEDBPM() {
  static double last_led;
  static byte ledState = 0;

  double bpmCalc = (1 / (memory.bpm / 60) * 1000);
  if (millis() - last_led >= bpmCalc / matrixAlgoFator[memory.algo]) {
#if SHOW_SERIAL
    Serial.print("Algo: ");
    Serial.print(memory.algo);
    Serial.print(" ");
#endif
    for (byte l = 0; l < LED_NUMBER; l++) {
      digitalWrite(LED[l], matrixAlgoLED[memory.algo][ledState][l]);
#if SHOW_SERIAL
      Serial.print(matrixAlgoLED[memory.algo][ledState][l]);
      Serial.print(" ");
#endif
    }
    showBPMTime((ledState / matrixAlgoFator[memory.algo]) + 1);

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
  static byte lastButton = BUTTON_NULL;
  byte actualButton = readButton();

  // para simular o efeito soltar botão,
  // para que a ação seja somente feita depois que o botão foi solto
  lastButton = actButton(actualButton, lastButton);
}

byte actButton(byte actualButton, byte lastButton) {
  static double last = 0;

#if SHOW_SERIAL
  Serial.print("Actual Button: ");
  Serial.print(actualButton);
  Serial.print(" Last Button: ");
  Serial.println(lastButton);
#endif

  if (actualButton != BUTTON_NULL && lastButton == actualButton ) {
#if SHOW_SERIAL
    Serial.print("---------------32--------------------: ");
    Serial.print(last);
    Serial.print("  ");
    Serial.print(millis());
    Serial.println(" last and actual equal");
#endif
    if ( millis() - last > BUTTON_TIME) {
      switch (lastButton ) {
        case BUTTON_RIGHT:
          writeBPMProg(BPM_PROG_RIGHT);
          break;
        case BUTTON_LEFT:
          writeBPMProg(BPM_PROG_LEFT);
          break;
        case BUTTON_UP:
          if (memory.sensor == SENSOR_POTENCIOMETER)
            writeBPMProg(BPM_PROG_UP);
          break;
        case BUTTON_DOWN:
          if (memory.sensor == SENSOR_POTENCIOMETER)
            writeBPMProg(BPM_PROG_DOWN);
          break;
      }
      last = 0;
    }
  } else if (actualButton == BUTTON_NULL && lastButton != BUTTON_NULL) {
#if SHOW_SERIAL
    Serial.print("---------------33--------------------");
    Serial.println("last new button and actual equal null");
#endif
    switch (lastButton ) {
      case BUTTON_RIGHT:
        readBPMProg(BPM_PROG_RIGHT);
        break;
      case BUTTON_LEFT:
        readBPMProg(BPM_PROG_LEFT);
        break;
      case BUTTON_UP:
        if (memory.sensor == SENSOR_BUTTONS) upBPM();
        else readBPMProg(BPM_PROG_UP);
        break;
      case BUTTON_DOWN:
        if (memory.sensor == SENSOR_BUTTONS) downBPM();
        else readBPMProg(BPM_PROG_DOWN);
        break;
      case BUTTON_SELECT:
        memory.algo = ++memory.algo >= LED_ALGO ? 0 : memory.algo;
        memoryChanged = millis();
        break;
    }
    last = 0;
  }
  else  last = millis();
  return actualButton;
}

void writeBPMProg(byte prog) {
  memory.bpmProg[prog] = memory.bpm;
  memoryChanged = millis();
}
void readBPMProg(byte prog) {
  memory.bpm = memory.bpmProg[prog];
}

void upBPM() {
  memory.bpm++;
  memory.bpm = min(BPM_MAX, memory.bpm);
  memoryChanged = millis();
}

void downBPM() {
  memory.bpm--;
  memory.bpm = max(BPM_MIN, memory.bpm);
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
  return BUTTON_NULL;
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

void readMemory() {
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
