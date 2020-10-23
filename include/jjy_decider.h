#ifndef Arduino_JJY_Decider_h
#define Arduino_JJY_Decider_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "jjy_datetime.h"
#include "jjy_decoder.h"

#define JJY_DECIDER_CANDIDATES_COUNT 2

class JJYDecider {
 public:
  void reset();
  void countUp();
  void setTimeCode(JJYDecoder *decoder, uint8_t seconds, uint8_t timecode);

  String dump(uint8_t seconds);
  JJYDateTime *getDecidedDateTime();

 private:
  JJYDateTime candidates[JJY_DECIDER_CANDIDATES_COUNT + 1];
  JJYDateTime decided;

  void decide();
};

#endif
