#ifndef Arduino_Synchronizer_h
#define Arduino_Synchronizer_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#define SYNCHRONIZER_TOLERANCE_COUNT 3
#define SYNCHRONIZER_TOLERANCE_PERCENT 5.0
#define SYNCHRONIZER_UPPER_THRESHOLD (1000000 + 1000000 * (SYNCHRONIZER_TOLERANCE_PERCENT / 100.0))
#define SYNCHRONIZER_LOWER_THRESHOLD (1000000 - 1000000 * (SYNCHRONIZER_TOLERANCE_PERCENT / 100.0))

#define interval_in_tolerance(interval) \
  ((interval) >= SYNCHRONIZER_LOWER_THRESHOLD && (interval) <= SYNCHRONIZER_UPPER_THRESHOLD)

class __synchronizer {
 public:
  static void begin(uint8_t pin, void (*callback)(void));

 private:
  static uint8_t pin;
  static void (*callback)(void);

  static bool synchronized;
  static uint32_t old_time;
  static uint32_t old_calltime;
  static uint8_t tolerance_interval_count;

  static void interrupt_callback();
  static void synchronized_callback();
};

extern __synchronizer Synchronizer;

#endif
