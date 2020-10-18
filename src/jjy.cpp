#include "jjy.h"

char get_jjy_char(uint8_t jjy_signal) {
  switch (jjy_signal) {
    case JJY_L:
      return 'L';
    case JJY_H:
      return 'H';
    case JJY_M:
      return 'M';
    default:
      return '?';
  }
}
