#ifndef SEVEN_SEGMENT_DISPLAY_H
#define SEVEN_SEGMENT_DISPLAY_H

#include "../Task/Task.h"
#include "../DigitalOutput/DigitalOutput.h"

#include <stdint.h>

class SevenSegmentDisplay {
public:
  SevenSegmentDisplay(DigitalOutput &top,
                      DigitalOutput &upper_right,
                      DigitalOutput &lower_right,
                      DigitalOutput &bottom,
                      DigitalOutput &lower_left,
                      DigitalOutput &upper_left,
                      DigitalOutput &middle,
                      DigitalOutput &dot);
  virtual bool init();

  virtual bool setNumber(unsigned int number);
  virtual bool setCharacter(char c);
  virtual bool setDot(bool on);
  virtual bool off();

private:
  enum Segment {
    kTop = 1,
    kUpperRight = 2,
    kLowerRight = 4,
    kBottom = 8,
    kLowerLeft = 16,
    kUpperLeft = 32,
    kMiddle = 64
  };

  static const uint8_t number_lookup[10];
  static const uint8_t upper_case_lookup[26];
  static const uint8_t lower_case_lookup[26];

  bool setSegments(uint8_t c);

  DigitalOutput &dot;
  DigitalOutput *outputs[7];
};

#endif /* SEVEN_SEGMENT_DISPLAY_H */
