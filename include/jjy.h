#ifndef Arduino_JJY_h
#define Arduino_JJY_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#define JJY_U 0
#define JJY_L 1
#define JJY_H 2
#define JJY_M 3

char get_jjy_char(uint8_t jjy_signal);

#endif
