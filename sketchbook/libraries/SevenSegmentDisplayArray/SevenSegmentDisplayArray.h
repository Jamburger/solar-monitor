#ifndef SEVEN_SEGMENT_DISPLAY_ARRAY_H
#define SEVEN_SEGMENT_DISPLAY_ARRAY_H

#include "../SevenSegmentDisplay/SevenSegmentDisplay.h"

#include <stdlib.h>

template <size_t Count>
class SevenSegmentDisplayArray {
public:
  SevenSegmentDisplayArray(SevenSegmentDisplay **displays) {
    for (size_t i = 0; i < Count; ++i) {
      this->displays[i] = *(displays++);
    }
  }

  SevenSegmentDisplay *get(size_t i) {
    if (i < Count) {
      return displays[i];
    }
    return NULL;
  }

private:
  SevenSegmentDisplay *displays[Count];
};

#endif /* SEVEN_SEGMENT_DISPLAY_ARRAY_H */
