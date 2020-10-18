#include "jjy_decider.h"

void JJYDecider::reset() {
  this->decided.reset();

  for (uint8_t i = 0; i < JJY_DECIDER_CANDIDATES_COUNT + 1; i++) { this->candidates[i].reset(); }
}

void JJYDecider::countUp() {
  this->decided.countUp();

  for (uint8_t i = 0; i < JJY_DECIDER_CANDIDATES_COUNT + 1; i++) { this->candidates[i].countUp(); }
  for (uint8_t i = JJY_DECIDER_CANDIDATES_COUNT; i >= 1; i--) { this->candidates[i] = this->candidates[i - 1]; }
}

void JJYDecider::setTimeCode(JJYDecoder *decoder, uint8_t seconds, uint8_t timecode) {
  decoder->setTimeCode(seconds, timecode);
  decoder->toDatetime(&candidates[0]);

  this->decide();
}

String JJYDecider::dump(uint8_t seconds) {
  seconds     = seconds % 60;
  String text = String("");

  for (uint8_t i = 0; i < JJY_DECIDER_CANDIDATES_COUNT + 1; i++) {
    if (i > 0)
      text += ", ";

    text += String(i, 10) + ": " + this->candidates[i].toString(seconds);
  }

  return text + " ==> " + this->decided.toString(seconds);
}

void JJYDecider::decide() {
  int8_t minutes         = this->candidates[0].getMinutes();
  int8_t hours           = this->candidates[0].getHours();
  int16_t totalDates     = this->candidates[0].getTotalDates();
  int8_t summerTimeInfo  = this->candidates[0].getSummerTimeInfo();
  int16_t year           = this->candidates[0].getYear();
  int8_t day             = this->candidates[0].getDay();
  int8_t leapSecondsInfo = this->candidates[0].getLeapSecondsInfo();

  bool isDecidableMinutes         = minutes != JJY_DATETIME_UNKNOWN;
  bool isDecidableHours           = hours != JJY_DATETIME_UNKNOWN;
  bool isDecidableTotalDates      = totalDates != JJY_DATETIME_UNKNOWN;
  bool isDecidableSummerTimeInfo  = summerTimeInfo != JJY_DATETIME_UNKNOWN;
  bool isDecidableYear            = year != JJY_DATETIME_UNKNOWN;
  bool isDecidableDay             = day != JJY_DATETIME_UNKNOWN;
  bool isDecidableLeapSecondsInfo = leapSecondsInfo != JJY_DATETIME_UNKNOWN;

  for (uint8_t i = 1; i < JJY_DECIDER_CANDIDATES_COUNT + 1; i++) {
    isDecidableMinutes &= minutes == this->candidates[i].getMinutes();
    isDecidableHours &= hours == this->candidates[i].getHours();
    isDecidableTotalDates &= totalDates == this->candidates[i].getTotalDates();
    isDecidableSummerTimeInfo &= summerTimeInfo == this->candidates[i].getSummerTimeInfo();
    isDecidableYear &= year == this->candidates[i].getYear();
    isDecidableDay &= day == this->candidates[i].getDay();
    isDecidableLeapSecondsInfo &= leapSecondsInfo == this->candidates[i].getLeapSecondsInfo();
  }

  if (isDecidableMinutes)
    this->decided.setMinutes(minutes);

  if (isDecidableHours)
    this->decided.setHours(hours);

  if (isDecidableTotalDates)
    this->decided.setTotalDates(totalDates);

  if (isDecidableSummerTimeInfo)
    this->decided.setSummerTimeInfo(summerTimeInfo);

  if (isDecidableYear)
    this->decided.setYear(year);

  if (isDecidableDay)
    this->decided.setDay(day);

  if (isDecidableLeapSecondsInfo)
    this->decided.setLeapSecondsInfo(leapSecondsInfo);
}
