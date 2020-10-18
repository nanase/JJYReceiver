#include "jjy_datetime.h"

void JJYDateTime::reset() {
  this->minutes          = JJY_DATETIME_UNKNOWN;
  this->hours            = JJY_DATETIME_UNKNOWN;
  this->totalDates       = JJY_DATETIME_UNKNOWN;
  this->parityOfHours    = JJY_DATETIME_UNKNOWN;
  this->parityOfMinutes  = JJY_DATETIME_UNKNOWN;
  this->summerTimeInfo   = JJY_DATETIME_UNKNOWN;
  this->year             = JJY_DATETIME_UNKNOWN;
  this->day              = JJY_DATETIME_UNKNOWN;
  this->leapSecondsInfo  = JJY_DATETIME_UNKNOWN;
  this->stopNotationInfo = JJY_DATETIME_UNKNOWN;
}

void JJYDateTime::countUp() {
  if (this->minutes != JJY_DATETIME_UNKNOWN) {
    this->minutes++;
  }

  if (this->minutes >= 60) {
    this->minutes = 0;

    if (this->hours != JJY_DATETIME_UNKNOWN) {
      this->hours++;
    }
  }

  if (this->hours >= 24) {
    this->hours = 0;

    if (this->totalDates != JJY_DATETIME_UNKNOWN) {
      this->totalDates++;
    }

    if (this->day != JJY_DATETIME_UNKNOWN) {
      this->day++;
    }
  }

  if (this->year != JJY_DATETIME_UNKNOWN) {
    if (this->isLeapYear()) {
      if (this->totalDates >= 366) {
        this->year++;
        this->totalDates = 1;
      }
    } else {
      if (this->totalDates >= 365) {
        this->year++;
        this->totalDates = 1;
      }
    }
  } else {
    if (this->totalDates >= 365) {
      this->year++;
      this->totalDates = 1;
    }
  }

  if (this->day != JJY_DATETIME_UNKNOWN) {
    this->day++;
  }

  if (this->day >= 7) {
    this->day = 0;
  }
}

// ----------------------

int8_t JJYDateTime::getMinutes() {
  return this->minutes;
}

int8_t JJYDateTime::getHours() {
  return this->hours;
}

int16_t JJYDateTime::getTotalDates() {
  return this->totalDates;
}

int8_t JJYDateTime::getParityOfHours() {
  return this->parityOfHours;
}

int8_t JJYDateTime::getParityOfMinutes() {
  return this->parityOfMinutes;
}

int8_t JJYDateTime::getSummerTimeInfo() {
  return this->summerTimeInfo;
}

int16_t JJYDateTime::getYear() {
  return this->year;
}

int8_t JJYDateTime::getDay() {
  return this->day;
}

int8_t JJYDateTime::getLeapSecondsInfo() {
  return this->leapSecondsInfo;
}

int8_t JJYDateTime::getStopNotationInfo() {
  return this->stopNotationInfo;
}

// -------------------

void JJYDateTime::setMinutes(int8_t minutes) {
  this->minutes = minutes;
}

void JJYDateTime::setHours(int8_t hours) {
  this->hours = hours;
}

void JJYDateTime::setTotalDates(int16_t totalDates) {
  this->totalDates = totalDates;
}

void JJYDateTime::setParityOfHours(int8_t parityOfHours) {
  this->parityOfHours = parityOfHours;
}

void JJYDateTime::setParityOfMinutes(int8_t parityOfMinutes) {
  this->parityOfMinutes = parityOfMinutes;
}

void JJYDateTime::setSummerTimeInfo(int8_t summerTimeInfo) {
  this->summerTimeInfo = summerTimeInfo;
}

void JJYDateTime::setYear(int16_t year) {
  this->year = year;
}

void JJYDateTime::setDay(int8_t day) {
  this->day = day;
}

void JJYDateTime::setLeapSecondsInfo(int8_t leapSecondsInfo) {
  this->leapSecondsInfo = leapSecondsInfo;
}

void JJYDateTime::setStopNotationInfo(int8_t stopNotationInfo) {
  this->stopNotationInfo = stopNotationInfo;
}

// -------------------

bool JJYDateTime::isLeapYear() {
  if (this->year == JJY_DATETIME_UNKNOWN)
    return false;

  return this->year % 400 == 0 || (this->year % 100 != 0 && this->year % 4 == 0);
}

// -------------------

String fillZero(uint8_t value) {
  if (value < 10) {
    return String("0") + String(value, 10);
  }

  return String(value, 10);
}

String JJYDateTime::toTimeString(uint8_t seconds) {
  return (this->hours == JJY_DATETIME_UNKNOWN ? "--" : fillZero(this->hours)) + String(":") +
         (this->minutes == JJY_DATETIME_UNKNOWN ? "--" : fillZero(this->minutes)) + String(":") + fillZero(seconds);
}

String JJYDateTime::toDateString() {
  int8_t month = JJY_DATETIME_UNKNOWN;
  int8_t date  = JJY_DATETIME_UNKNOWN;

  if (this->year != JJY_DATETIME_UNKNOWN && this->totalDates != JJY_DATETIME_UNKNOWN)
    JJYDateTime::toDate(this->isLeapYear(), this->totalDates, &month, &date);

  return (this->year == JJY_DATETIME_UNKNOWN ? "----" : String(this->year + 2000)) + String("/") +
         (month == JJY_DATETIME_UNKNOWN ? "--" : fillZero(month)) + String("/") +
         (date == JJY_DATETIME_UNKNOWN ? "--" : fillZero(date));
}

String JJYDateTime::toString(uint8_t seconds) {
  return this->toDateString() + String(" ") + this->toTimeString(seconds);
}

// -------------------

const uint8_t month_dates[]           = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const uint8_t month_dates_leap_year[] = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

void JJYDateTime::toDate(bool leapYear, uint16_t totalDates, int8_t *month, int8_t *date) {
  *month = 0;

  if (leapYear) {
    do {
      totalDates -= month_dates_leap_year[*month];
      (*month)++;
    } while (totalDates > month_dates_leap_year[*month]);
  } else {
    do {
      totalDates -= month_dates[*month];
      (*month)++;
    } while (totalDates > month_dates[*month]);
  }

  *date = totalDates;
}
