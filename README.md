# 📘 Práctica 2B – Interrupciones por Timer (ESP32)

## 📖 Descripción

En esta práctica se estudia el uso de **interrupciones mediante temporizadores (timers)** en el microcontrolador ESP32.

Se configura un temporizador hardware que genera interrupciones periódicas automáticamente, permitiendo ejecutar código a intervalos de tiempo precisos sin depender del bucle principal (`loop()`).

---

## 🎯 Objetivos

* Comprender el funcionamiento de los temporizadores en ESP32.
* Implementar interrupciones periódicas mediante timers.
* Gestionar variables compartidas entre ISR y programa principal.
* Mostrar resultados en el monitor serie.

---

## 🛠️ Material necesario

* ESP32
* Ordenador con Visual Studio Code + PlatformIO

---

## 💻 Código

```cpp id="code2b1"
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
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true); // 1 segundo
  timerAlarmEnable(timer);
}

void loop() {

  if (interruptCounter > 0) {

    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);

    totalInterruptCounter++;

    Serial.print("An interrupt has occurred. Total number: ");
    Serial.println(totalInterruptCounter);
  }
}
```

---

## ⚙️ Configuración (PlatformIO)

Archivo `platformio.ini`:

```ini id="code2b2"
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino

monitor_speed = 115200
```

---

## ▶️ Funcionamiento

1. Se configura un temporizador hardware del ESP32.
2. El timer genera una interrupción cada **1 segundo**.
3. Se ejecuta la ISR (`onTimer()`).
4. Se incrementa un contador de interrupciones.
5. En el `loop()`, se procesa y se muestra el resultado por el monitor serie.

---

## 🧠 Conceptos clave

* **Timer hardware:** contador interno que genera eventos periódicos.
* **ISR (Interrupt Service Routine):** función que se ejecuta al producirse la interrupción.
* **volatile:** necesario para variables modificadas dentro de la ISR.
* **portMUX:** mecanismo para evitar conflictos entre CPU y la interrupción.
* **Prescaler:** divide la frecuencia del reloj del microcontrolador.

---

## ⏱️ Configuración del tiempo

El periodo del timer depende de:

* Frecuencia base: **80 MHz**
* Prescaler: `80` → 1 MHz (1 µs por tick)
* Valor de alarma: `1000000` → **1 segundo**

---

## 📊 Ejemplo de salida

```id="code2b3"
An interrupt has occurred. Total number: 1
An interrupt has occurred. Total number: 2
An interrupt has occurred. Total number: 3
...
```

---

## ✅ Conclusión

Las interrupciones por timer permiten ejecutar tareas periódicas con gran precisión sin bloquear el programa principal. Esto es especialmente útil en sistemas embebidos donde el tiempo es crítico.

---

## 👨‍💻 Autor

- Oriol Vall Solé

---
