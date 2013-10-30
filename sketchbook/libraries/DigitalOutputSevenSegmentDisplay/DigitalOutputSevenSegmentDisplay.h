#ifndef DIGITAL_OUTPUT_SEVEN_SEGMENT_DISPLAY
#define DIGITAL_OUTPUT_SEVEN_SEGMENT_DISPLAY

#include "../SevenSegmentDisplay/SevenSegmentDisplay.h"

template <typename DigitalOutputType>
class DigitalOutputSevenSegmentDisplay : public SevenSegmentDisplay {
public:
  DigitalOutputSevenSegmentDisplay(unsigned int top_pin,
                                   unsigned int upper_right_pin,
                                   unsigned int lower_right_pin,
                                   unsigned int bottom_pin,
                                   unsigned int lower_left_pin,
                                   unsigned int upper_left_pin,
                                   unsigned int middle_pin,
                                   unsigned int decimal_pin) :
    SevenSegmentDisplay(top, upper_right, lower_right, bottom, lower_left,
        upper_left, middle, decimal), top(top_pin),
    upper_right(upper_right_pin),
    lower_right(lower_right_pin),
    bottom(bottom_pin),
    lower_left(lower_left_pin),
    upper_left(upper_left_pin),
    middle(middle_pin),
    decimal(decimal_pin) {
  }

  virtual bool init() {
    return (top.init() && upper_right.init() && lower_right.init() &&
            bottom.init() && lower_left.init() && upper_left.init() &&
            middle.init() && decimal.init() && SevenSegmentDisplay::init());
  }

private:
  DigitalOutputType top;
  DigitalOutputType upper_right;
  DigitalOutputType lower_right;
  DigitalOutputType bottom;
  DigitalOutputType lower_left;
  DigitalOutputType upper_left;
  DigitalOutputType middle;
  DigitalOutputType decimal;
};

#endif /* DIGITAL_OUTPUT_SEVEN_SEGMENT_DISPLAY */
