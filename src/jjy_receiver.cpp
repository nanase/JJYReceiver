#include "jjy_receiver.h"

void JJYReceiver::initialize(uint8_t pin_signal) {
  this->pin_signal = pin_signal;

  this->reset();
}

void JJYReceiver::reset() {
  this->signal          = 0;
  this->seconds         = 0;
  this->marker_count    = 0;
  this->marker_detected = false;

  this->state           = __JJYReceiver_InitialState;
  this->waitStartMicros = 0;

  this->decoder.reset();
  this->decider.reset();
}

void JJYReceiver::countUpSeconds() {
  this->seconds = (this->seconds + 1) % 60;
  this->state   = __JJYReceiver_InterruptedState;
}

void JJYReceiver::receive_signal() {
  this->signal |= digitalRead(this->pin_signal) ? 0x1 : 0x0;
  this->signal <<= 1;
}

void JJYReceiver::update() {
  switch (this->state) {
    case __JJYReceiver_InterruptedState: {
      this->waitStartMicros = micros();
      this->state           = __JJYReceiver_WaitState;
      this->signal          = 0;
      break;
    }

    case __JJYReceiver_WaitState: {
      if (micros() - this->waitStartMicros >= 350000) {  // after +350ms
        this->receive_signal();                          // read 1st bit
        this->waitStartMicros = micros();
        this->state           = __JJYReceiver_AfterRead0State;
      }
      break;
    }

    case __JJYReceiver_AfterRead0State: {
      if (micros() - this->waitStartMicros >= 100000) {  // after +100ms
        this->receive_signal();                          // read 2nd bit
        this->waitStartMicros = micros();
        this->state           = __JJYReceiver_AfterRead1State;
      }
      break;
    }

    case __JJYReceiver_AfterRead1State: {
      if (micros() - this->waitStartMicros >= 100000) {  // after +100ms
        this->receive_signal();                          // read 3rd bit
        this->waitStartMicros = micros();
        this->state           = __JJYReceiver_AfterRead2State;
      }
      break;
    }

    case __JJYReceiver_AfterRead2State: {
      if (micros() - this->waitStartMicros >= 100000) {  // after +100ms
        this->receive_signal();                          // read 4th bit
        this->signal >>= 1;

        this->state  = __JJYReceiver_EndOfState;
        uint8_t code = JJYReceiver::judgeCode(this->signal);

        if (code == JJY_M)
          this->marker_count++;
        else
          this->marker_count = 0;

        if (this->marker_count == 2) {
          this->seconds         = 0;
          this->marker_detected = true;
        }

        if ((this->seconds + 1) % 60 == 0)
          this->decider.countUp();

        if (this->marker_detected)
          this->decider.setTimeCode(&this->decoder, this->seconds, code);

        if (this->onReceiveTimeCode != NULL)
          this->onReceiveTimeCode(code);
      }
      break;
    }

    // case __JJYReceiver_InitialState:
    // case __JJYReceiver_EndOfState:
    default:
      break;
  }
}

JJYDecoder *JJYReceiver::getJJYDecoder() {
  return &this->decoder;
}

JJYDecider *JJYReceiver::getJJYDecider() {
  return &this->decider;
}

uint8_t JJYReceiver::getSeconds() {
  return (this->seconds + 1) % 60;
}

bool JJYReceiver::markerDetected() {
  return this->marker_detected;
}

void JJYReceiver::attachOnReceiveTimeCode(void (*callback)(uint8_t timecode)) {
  this->onReceiveTimeCode = callback;
}

// -----------------------

const uint8_t JJY_JUDGE_MAP[16] = { JJY_M, JJY_U, JJY_U, JJY_U, JJY_H, JJY_U, JJY_U, JJY_L,
                                    JJY_H, JJY_L, JJY_H, JJY_L, JJY_H, JJY_L, JJY_H, JJY_L };

uint8_t JJYReceiver::judgeCode(uint8_t signal) {
  return JJY_JUDGE_MAP[signal & 0b1111];
}
