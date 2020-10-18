#include <Arduino.h>

#include "jjy_receiver.h"
#include "synchronizer.h"

#define PIN_LED 13
#define PIN_SPEAKER 19
#define PIN_JJY_SIGNAL 2

JJYReceiver receiver;

void callback() {
  receiver.countUpSeconds();
  tone(PIN_SPEAKER, 800, 50);
}

void setup() {
  Serial.begin(9600);

  pinMode(PIN_SPEAKER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_SPEAKER, LOW);

  receiver.initialize(PIN_JJY_SIGNAL);
  Synchronizer.begin(PIN_JJY_SIGNAL, callback);
}

void loop() {
  receiver.update();
}
