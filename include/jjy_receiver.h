#ifndef Arduino_JJY_Receiver_h
#define Arduino_JJY_Receiver_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "jjy.h"
#include "jjy_decider.h"
#include "jjy_decoder.h"

enum {
  __JJYReceiver_InitialState     = 0,
  __JJYReceiver_InterruptedState = 1,
  __JJYReceiver_WaitState        = 2,
  __JJYReceiver_AfterRead0State  = 3,
  __JJYReceiver_AfterRead1State  = 4,
  __JJYReceiver_AfterRead2State  = 5,
  __JJYReceiver_EndOfState       = 6,
};

class JJYReceiver {
 public:
  void initialize(uint8_t pin_signal);
  void reset();

  void countUpSeconds();
  void update();

  JJYDecoder *getJJYDecoder();
  JJYDecider *getJJYDecider();
  uint8_t getSeconds();
  bool markerDetected();

  void attachOnReceiveTimeCode(void (*callback)(uint8_t timecode));

  static uint8_t judgeCode(uint8_t signal);

 private:
  JJYDecoder decoder;
  JJYDecider decider;

  uint8_t state            = __JJYReceiver_InitialState;
  uint32_t waitStartMicros = 0;

  uint8_t pin_signal       = 0;
  uint8_t signal           = 0;
  volatile uint8_t seconds = 0;
  uint8_t marker_count     = 0;
  bool marker_detected     = false;

  void (*onReceiveTimeCode)(uint8_t timecode) = NULL;

  void receive_signal();
};

#endif
