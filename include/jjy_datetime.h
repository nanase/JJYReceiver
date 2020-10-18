#ifndef Arduino_JJY_Datetime_h
#define Arduino_JJY_Datetime_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#define JJY_DATETIME_UNKNOWN (-1)

class JJYDateTime {
 public:
  void reset();
  void countUp();

  int8_t getMinutes();
  int8_t getHours();
  int16_t getTotalDates();
  int8_t getParityOfHours();
  int8_t getParityOfMinutes();
  int8_t getSummerTimeInfo();
  int16_t getYear();
  int8_t getDay();
  int8_t getLeapSecondsInfo();
  int8_t getStopNotationInfo();

  void setMinutes(int8_t minutes);
  void setHours(int8_t hours);
  void setTotalDates(int16_t totalDates);
  void setParityOfHours(int8_t parityOfHours);
  void setParityOfMinutes(int8_t parityOfMinutes);
  void setSummerTimeInfo(int8_t summerTimeInfo);
  void setYear(int16_t year);
  void setDay(int8_t day);
  void setLeapSecondsInfo(int8_t leapSecondsInfo);
  void setStopNotationInfo(int8_t stopNotationInfo);

  bool isLeapYear();

  String toTimeString(uint8_t seconds);
  String toDateString();
  String toString(uint8_t seconds);

  static void toDate(bool leapYear, uint16_t totalDates, int8_t *month, int8_t *date);

 private:
  int8_t minutes          = JJY_DATETIME_UNKNOWN;
  int8_t hours            = JJY_DATETIME_UNKNOWN;
  int16_t totalDates      = JJY_DATETIME_UNKNOWN;
  int8_t parityOfHours    = JJY_DATETIME_UNKNOWN;
  int8_t parityOfMinutes  = JJY_DATETIME_UNKNOWN;
  int8_t summerTimeInfo   = JJY_DATETIME_UNKNOWN;
  int16_t year            = JJY_DATETIME_UNKNOWN;
  int8_t day              = JJY_DATETIME_UNKNOWN;
  int8_t leapSecondsInfo  = JJY_DATETIME_UNKNOWN;
  int8_t stopNotationInfo = JJY_DATETIME_UNKNOWN;
};

#endif
