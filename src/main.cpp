#include <Arduino.h>

// Variables compartidas con la ISR
volatile int interruptCounter = 0;
int totalInterruptCounter = 0;

// Timer
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// ISR del timer
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {

  Serial.begin(115200);

  // Configuración del timer
  timer = timerBegin(0, 80, true);              // Timer 0, prescaler 80 → 1 tick = 1µs
  timerAttachInterrupt(timer, &onTimer, true);  // Asociar ISR
  timerAlarmWrite(timer, 1000000, true);        // 1.000.000 µs = 1 segundo
  timerAlarmEnable(timer);                      // Activar timer
}

void loop() {

  // Comprobar si ha habido interrupciones
  if (interruptCounter > 0) {

    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);

    totalInterruptCounter++;

    Serial.print("An interrupt has occurred. Total number: ");
    Serial.println(totalInterruptCounter);
  }
}