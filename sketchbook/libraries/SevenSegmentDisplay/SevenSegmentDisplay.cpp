#include "SevenSegmentDisplay.h"
#include "../DigitalOutput/DigitalOutput.h"

#include <stdint.h>

const uint8_t SevenSegmentDisplay::number_lookup[10] = {
  // 0
  kTop | kUpperRight | kLowerRight | kBottom | kLowerLeft | kUpperLeft,

  // 1
  kUpperRight | kLowerRight,

  // 2
  kTop | kUpperRight | kMiddle | kLowerLeft | kBottom,

  // 3
  kTop | kUpperRight | kMiddle | kLowerRight | kBottom,

  // 4
  kUpperRight | kLowerRight | kUpperLeft | kMiddle,

  // 5
  kLowerRight | kBottom | kUpperLeft | kTop | kMiddle,

  // 6
  kTop | kLowerRight | kBottom | kLowerLeft | kUpperLeft | kMiddle,

  // 7
  kTop | kUpperRight | kLowerRight,

  // 8
  kTop | kUpperRight | kLowerRight | kBottom | kLowerLeft | kUpperLeft |
      kMiddle,

  // 9
  kTop | kUpperRight | kLowerRight | kBottom | kUpperLeft | kMiddle
};

const uint8_t SevenSegmentDisplay::upper_case_lookup[26] = {
  // A
  kTop | kUpperRight | kLowerRight | kLowerLeft | kUpperLeft | kMiddle,

  // B
  kTop | kUpperRight | kLowerRight | kBottom | kLowerLeft | kUpperLeft |
      kMiddle,

  // C
  kTop | kBottom | kLowerLeft | kUpperLeft,

  // D
  kTop | kUpperRight | kLowerRight | kBottom | kLowerLeft | kUpperLeft,

  // E
  kTop | kBottom | kLowerLeft | kUpperLeft | kMiddle,

  // F
  kTop | kLowerLeft | kUpperLeft | kMiddle,

  // G
  kTop | kLowerRight | kBottom | kLowerLeft | kUpperLeft | kMiddle,

  // H
  kUpperRight | kLowerRight | kLowerLeft | kUpperLeft | kMiddle,

  // I
  kLowerLeft | kUpperLeft,

  // J
  kLowerRight | kBottom | kLowerLeft | kUpperLeft,

  // K
  kUpperRight | kLowerRight | kLowerLeft | kUpperLeft | kMiddle,

  // L
  kBottom | kLowerLeft | kUpperLeft,

  // M
  kTop | kLowerRight | kLowerLeft | kMiddle,

  // N
  kTop | kUpperRight | kLowerRight | kLowerLeft | kUpperLeft,

  // O
  kTop | kUpperRight | kLowerRight | kBottom | kLowerLeft | kUpperLeft,

  // P
  kTop | kUpperRight | kLowerLeft | kUpperLeft | kMiddle,

  // Q
  kTop | kUpperRight | kLowerRight | kUpperLeft | kMiddle,

  // R
  kTop | kLowerLeft | kUpperLeft,

  // S
  kTop | kLowerRight | kBottom | kUpperLeft | kMiddle,

  // T
  kTop | kLowerLeft | kUpperLeft,

  // U
  kUpperRight | kLowerRight | kBottom | kLowerLeft | kUpperLeft,

  // V
  kUpperRight | kLowerRight | kBottom | kLowerLeft | kUpperLeft,

  // W
  kUpperRight | kBottom | kUpperLeft | kMiddle,

  // X
  kUpperRight | kLowerRight | kLowerLeft | kUpperLeft | kMiddle,

  // Y
  kUpperRight | kLowerRight | kBottom | kUpperLeft | kMiddle,

  // Z
  kTop | kUpperRight | kMiddle | kLowerLeft | kBottom
};

const uint8_t SevenSegmentDisplay::lower_case_lookup[26] = {
  // a
  kTop | kUpperRight | kLowerRight | kBottom | kLowerLeft | kMiddle,

  // b
  kLowerRight | kBottom | kLowerLeft | kUpperLeft | kMiddle,

  // c
  kBottom | kLowerLeft | kMiddle,

  // d
  kUpperRight | kLowerRight | kBottom | kLowerLeft | kMiddle,

  // e
  kTop | kUpperRight | kBottom | kLowerLeft | kUpperLeft | kMiddle,

  // f
  kTop | kLowerLeft | kUpperLeft | kMiddle,

  // g
  kTop | kUpperRight | kLowerRight | kBottom | kUpperLeft | kMiddle,

  // h
  kLowerRight | kLowerLeft | kMiddle | kUpperLeft,

  // i
  kLowerLeft,

  // j
  kLowerLeft | kBottom,

  // k
  kUpperRight | kLowerRight | kLowerLeft | kUpperLeft | kMiddle,

  // l
  kLowerLeft | kUpperLeft,

  // m
  kMiddle | kBottom,

  // n
  kLowerLeft | kMiddle | kLowerRight,

  // o
  kLowerRight | kBottom | kLowerLeft | kMiddle,

  // p
  kTop | kUpperRight | kLowerLeft | kUpperLeft | kMiddle,

  // q
  kTop | kUpperRight | kLowerRight | kUpperLeft | kMiddle,

  // r
  kLowerLeft | kMiddle,

  // s
  kTop | kLowerRight | kBottom | kUpperLeft | kMiddle,

  // t
  kBottom | kLowerLeft | kMiddle | kUpperLeft,

  // u
  kBottom | kLowerLeft | kLowerRight,

  // v
  kBottom | kLowerLeft | kLowerRight,

  // w
  kUpperRight | kBottom | kUpperLeft | kMiddle,

  // x
  kUpperRight | kLowerRight | kLowerLeft | kUpperLeft | kMiddle,

  // y
  kUpperRight | kLowerRight | kBottom | kUpperLeft | kMiddle,

  // z
  kTop | kUpperRight | kMiddle | kLowerLeft | kBottom
};

SevenSegmentDisplay::SevenSegmentDisplay(DigitalOutput &top,
                                         DigitalOutput &upper_right,
                                         DigitalOutput &lower_right,
                                         DigitalOutput &bottom,
                                         DigitalOutput &lower_left,
                                         DigitalOutput &upper_left,
                                         DigitalOutput &middle,
                                         DigitalOutput &dot) :
      dot(dot) {
  outputs[0] = &top;
  outputs[1] = &upper_right;
  outputs[2] = &lower_right;
  outputs[3] = &bottom;
  outputs[4] = &lower_left;
  outputs[5] = &upper_left;
  outputs[6] = &middle;
}

bool SevenSegmentDisplay::init() {
  return off();
}

bool SevenSegmentDisplay::setNumber(unsigned int number) {
  if (number > 9) {
    return false;
  }
  return setCharacter('0' + number);
}

bool SevenSegmentDisplay::setCharacter(char c) {
  if (c >= '0' && c <= '9') {
    return setSegments(number_lookup[c - '0']);
  } else if (c >= 'A' && c <= 'Z') {
    return setSegments(upper_case_lookup[c - 'A']);
  } else if (c >= 'a' && c <= 'z') {
    return setSegments(lower_case_lookup[c - 'a']);
  } else {
    off();
    return false;
  }
}

bool SevenSegmentDisplay::setDot(bool on) {
  return dot.set(on);
}

bool SevenSegmentDisplay::off() {
  return (setSegments(0) && setDot(false));
}

bool SevenSegmentDisplay::setSegments(uint8_t c) {
  for (unsigned int i = 0; i < 7; ++i) {
    if (!outputs[i]->set(c & (1 << i))) {
      return false;
    }
  }
  return true;
}
