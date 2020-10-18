#ifndef Arduino_JJY_Decoder_h
#define Arduino_JJY_Decoder_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "jjy.h"
#include "jjy_datetime.h"

#define JJY_DECODER_TIMECODE_SIZE 61

class JJYDecoder {
 public:
  void reset();

  uint8_t getMinutes();
  uint8_t getHours();
  uint16_t getTotalDates();
  uint8_t getParityOfHours();
  uint8_t getParityOfMinutes();
  uint8_t getSummerTimeInfo();
  uint16_t getYear();
  uint8_t getDay();
  uint8_t getLeapSecondsInfo();
  uint8_t getStopNotationInfo();

  bool isDecodableMinutes();
  bool isDecodableHours();
  bool isDecodableTotalDates();
  bool isDecodableParityOfHours();
  bool isDecodableParityOfMinutes();
  bool isDecodableSummerTimeInfo();
  bool isDecodableYear();
  bool isDecodableDay();
  bool isDecodableLeapSecondsInfo();
  bool isDecodableStopNotationInfo();

  bool isCorrectParityOfHours();
  bool isCorrectParityOfMinutes();
  uint8_t getMarkerQuality();

  void setTimeCode(uint8_t seconds, uint8_t timecode);

  void toDatetime(JJYDateTime *datetime);

 private:
  uint8_t timecodes[JJY_DECODER_TIMECODE_SIZE];
};

#endif
