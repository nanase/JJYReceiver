#include <Arduino.h>

#include "synchronizer.h"

#define PIN_SPEAKER 19
#define PIN_JJY_SIGNAL 2

void callback() {
  tone(PIN_SPEAKER, 800, 50);
}

void setup() {
  Serial.begin(9600);

  pinMode(PIN_SPEAKER, OUTPUT);
  digitalWrite(PIN_SPEAKER, LOW);

  Synchronizer.begin(PIN_JJY_SIGNAL, callback);
}

void loop() {}
