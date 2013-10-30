#ifndef METER_H
#define METER_H

#include "../SevenSegmentDisplay/SevenSegmentDisplay.h"
#include "../SevenSegmentDisplayArray/SevenSegmentDisplayArray.h"

template <size_t Count>
class DecimalDisplay : public SevenSegmentDisplayArray<Count> {
public:
  DecimalDisplay(SevenSegmentDisplay **displays) :
      SevenSegmentDisplayArray<Count>(displays) {
  }

  bool init() {
    for (size_t i = 0; i < Count; ++i)  {
      if (!SevenSegmentDisplayArray<Count>::get(i)->off()) {
        return false;
      }
    }
    return true;
  }

  bool set(double value) {
    // First, find the starting power of 10. The power should not be less than
    // zero, since we always need to display the ones place.
    double ten_power, ten_power_plus_one;
    for (ten_power = 1, ten_power_plus_one = 10;
         value > ten_power && value > ten_power_plus_one;
         ten_power = ten_power_plus_one, ten_power_plus_one *= 10);

    // Now resolve and display each digit. When power is 0 and more digits
    // follow, a decimal point is displayed.
    double tmp = value;
    for (size_t i = 0; i < Count; ++i) {
      int digit = (int) (tmp / ten_power);
      SevenSegmentDisplayArray<Count>::get(i)->setNumber(digit);
      if (ten_power == 1 && i < Count - 1) {
        SevenSegmentDisplayArray<Count>::get(i)->setDot(true);
      } else {
        SevenSegmentDisplayArray<Count>::get(i)->setDot(false);
      }
      tmp -= digit * ten_power;
      ten_power /= 10;
    }
    return true;
  }
};

#endif /* METER_H */
