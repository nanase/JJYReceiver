#include "synchronizer.h"

#include <TimerOne.h>

__synchronizer Synchronizer;

uint8_t __synchronizer::pin            = 0;
void (*__synchronizer::callback)(void) = NULL;

bool __synchronizer::synchronized                = false;
uint32_t __synchronizer::old_time                = 0;
uint32_t __synchronizer::old_calltime            = 0;
uint8_t __synchronizer::tolerance_interval_count = 0;

void __synchronizer::begin(uint8_t pin, void (*callback)(void)) {
  __synchronizer::pin      = pin;
  __synchronizer::callback = callback;

  pinMode(pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin), __synchronizer::interrupt_callback, RISING);

  Timer1.initialize(1000000);
  Timer1.attachInterrupt(__synchronizer::synchronized_callback);
  Timer1.stop();
}

bool __synchronizer::isSynchronized() {
  return __synchronizer::synchronized;
}

// -----------------------

void __synchronizer::interrupt_callback() {
  uint32_t now     = micros();
  int32_t interval = now - __synchronizer::old_time;

  if (interval_in_tolerance(interval)) {
    if (__synchronizer::tolerance_interval_count < SYNCHRONIZER_TOLERANCE_COUNT) {
      __synchronizer::tolerance_interval_count++;
    } else {
      __synchronizer::synchronized             = true;
      __synchronizer::tolerance_interval_count = 0;
      Timer1.start();
    }
  } else if (interval >= SYNCHRONIZER_UPPER_THRESHOLD) {
    __synchronizer::synchronized             = false;
    __synchronizer::tolerance_interval_count = 0;
  }

  __synchronizer::old_time = now;
}

void __synchronizer::synchronized_callback() {
  uint32_t now     = micros();
  int32_t interval = now - __synchronizer::old_calltime;

  if (interval < 500000)
    return;

  __synchronizer::old_calltime = now;

  if (__synchronizer::callback != NULL)
    __synchronizer::callback();
}
