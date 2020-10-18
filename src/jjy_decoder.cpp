#include "jjy_decoder.h"

#define codeHBit(index, value) (this->timecodes[index] == JJY_H ? (value) : 0)
#define codeMBit(index, value) (this->timecodes[index] == JJY_M ? (value) : 0)
#define codeInvalid(index) (this->timecodes[index] == JJY_H || this->timecodes[index] == JJY_L)

void JJYDecoder::reset() {
  for (uint8_t i = 0; i < JJY_DECODER_TIMECODE_SIZE; i++) this->timecodes[i] = JJY_U;
}

// ----------------

uint8_t JJYDecoder::getMinutes() {
  return codeHBit(1, 40) + codeHBit(2, 20) + codeHBit(3, 10) + codeHBit(5, 8) + codeHBit(6, 4) + codeHBit(7, 2) +
         codeHBit(8, 1);
}

uint8_t JJYDecoder::getHours() {
  return codeHBit(12, 20) + codeHBit(13, 10) + codeHBit(15, 8) + codeHBit(16, 4) + codeHBit(17, 2) + codeHBit(18, 1);
}

uint16_t JJYDecoder::getTotalDates() {
  return codeHBit(22, 200) + codeHBit(23, 100) + codeHBit(25, 80) + codeHBit(26, 40) + codeHBit(27, 20) +
         codeHBit(28, 10) + codeHBit(30, 8) + codeHBit(31, 4) + codeHBit(32, 2) + codeHBit(33, 1);
}

uint8_t JJYDecoder::getParityOfHours() {
  return codeHBit(36, 1);
}

uint8_t JJYDecoder::getParityOfMinutes() {
  return codeHBit(37, 1);
}

uint8_t JJYDecoder::getSummerTimeInfo() {
  return codeHBit(38, 0x02) + codeHBit(39, 0x01);
}

uint16_t JJYDecoder::getYear() {
  return codeHBit(41, 80) + codeHBit(42, 40) + codeHBit(43, 20) + codeHBit(44, 10) + codeHBit(45, 8) + codeHBit(46, 4) +
         codeHBit(47, 2) + codeHBit(48, 1);
}

uint8_t JJYDecoder::getDay() {
  return codeHBit(50, 4) + codeHBit(51, 2) + codeHBit(52, 1);
}

uint8_t JJYDecoder::getLeapSecondsInfo() {
  return codeHBit(53, 0x02) + codeHBit(54, 0x01);
}

uint8_t JJYDecoder::getStopNotationInfo() {
  return codeHBit(50, 0x20) + codeHBit(51, 0x10) + codeHBit(52, 0x08) + codeHBit(53, 0x04) + codeHBit(54, 0x02) +
         codeHBit(55, 0x01);
}

// ---------------

bool JJYDecoder::isDecodableMinutes() {
  return codeInvalid(1) && codeInvalid(2) && codeInvalid(3) && codeInvalid(5) && codeInvalid(6) && codeInvalid(7) &&
         codeInvalid(8);
}

bool JJYDecoder::isDecodableHours() {
  return codeInvalid(12) && codeInvalid(13) && codeInvalid(15) && codeInvalid(16) && codeInvalid(17) && codeInvalid(18);
}

bool JJYDecoder::isDecodableTotalDates() {
  return codeInvalid(22) && codeInvalid(23) && codeInvalid(25) && codeInvalid(26) && codeInvalid(27) &&
         codeInvalid(28) && codeInvalid(30) && codeInvalid(31) && codeInvalid(32) && codeInvalid(33);
}

bool JJYDecoder::isDecodableParityOfHours() {
  return codeInvalid(36);
}

bool JJYDecoder::isDecodableParityOfMinutes() {
  return codeInvalid(37);
}

bool JJYDecoder::isDecodableLeapSecondsInfo() {
  return codeInvalid(38) && codeInvalid(39);
}

bool JJYDecoder::isDecodableYear() {
  return codeInvalid(41) && codeInvalid(42) && codeInvalid(43) && codeInvalid(44) && codeInvalid(45) &&
         codeInvalid(46) && codeInvalid(47) && codeInvalid(48);
}

bool JJYDecoder::isDecodableDay() {
  return codeInvalid(50) && codeInvalid(51) && codeInvalid(52);
}

bool JJYDecoder::isDecodableSummerTimeInfo() {
  return codeInvalid(53) && codeInvalid(54);
}

bool JJYDecoder::isDecodableStopNotationInfo() {
  return codeInvalid(50) && codeInvalid(51) && codeInvalid(52) && codeInvalid(53) && codeInvalid(54) && codeInvalid(55);
}

// ---------------

bool JJYDecoder::isCorrectParityOfHours() {
  return ((codeHBit(12, 1) + codeHBit(13, 1) + codeHBit(15, 1) + codeHBit(16, 1) + codeHBit(17, 1) + codeHBit(18, 1)) %
          2) == this->getParityOfHours();
}

bool JJYDecoder::isCorrectParityOfMinutes() {
  return ((codeHBit(1, 1) + codeHBit(2, 1) + codeHBit(3, 1) + codeHBit(5, 1) + codeHBit(6, 1) + codeHBit(7, 1) +
           codeHBit(8, 1)) %
          2) == this->getParityOfMinutes();
}

uint8_t JJYDecoder::getMarkerQuality() {
  return codeMBit(0, 1) + codeMBit(9, 1) + codeMBit(19, 1) + codeMBit(29, 1) + codeMBit(39, 1) + codeMBit(49, 1) +
         codeMBit(59, 1);
}

void JJYDecoder::toDatetime(JJYDateTime *datetime) {
  datetime->reset();

  datetime->setMinutes(this->isDecodableMinutes() ? this->getMinutes() : JJY_DATETIME_UNKNOWN);
  datetime->setHours(this->isDecodableHours() ? this->getHours() : JJY_DATETIME_UNKNOWN);
  datetime->setTotalDates(this->isDecodableTotalDates() ? this->getTotalDates() : JJY_DATETIME_UNKNOWN);
  datetime->setParityOfHours(this->isDecodableParityOfHours() ? this->getParityOfHours() : JJY_DATETIME_UNKNOWN);
  datetime->setParityOfMinutes(this->isDecodableParityOfMinutes() ? this->getParityOfMinutes() : JJY_DATETIME_UNKNOWN);
  datetime->setSummerTimeInfo(this->isDecodableSummerTimeInfo() ? this->getSummerTimeInfo() : JJY_DATETIME_UNKNOWN);
  datetime->setYear(this->isDecodableYear() ? this->getYear() : JJY_DATETIME_UNKNOWN);
  datetime->setDay(this->isDecodableDay() ? this->getDay() : JJY_DATETIME_UNKNOWN);
  datetime->setLeapSecondsInfo(this->isDecodableLeapSecondsInfo() ? this->getLeapSecondsInfo() : JJY_DATETIME_UNKNOWN);
  datetime->setStopNotationInfo(this->isDecodableStopNotationInfo() ? this->getStopNotationInfo()
                                                                    : JJY_DATETIME_UNKNOWN);
}

// ---------------

void JJYDecoder::setTimeCode(uint8_t seconds, uint8_t timecode) {
  this->timecodes[seconds % JJY_DECODER_TIMECODE_SIZE] = timecode;
}
