#ifndef CHARACTER_DISPLAY_H
#define CHARACTER_DISPLAY_H

#include "../SevenSegmentDisplay/SevenSegmentDisplay.h"
#include "../SevenSegmentDisplayArray/SevenSegmentDisplayArray.h"

template <size_t Count>
class CharacterDisplay : public SevenSegmentDisplayArray<Count> {
public:
  CharacterDisplay(SevenSegmentDisplay **displays) :
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

  // Set the display to be the first Count characters of the provided string.
  bool set(const char * str) {
    for (size_t i = 0; i < Count; ++i) {
      if (*str) {
        SevenSegmentDisplayArray<Count>::get(i)->setCharacter(*str);
        SevenSegmentDisplayArray<Count>::get(i)->setDot(false);
        ++str;
      } else {
        SevenSegmentDisplayArray<Count>::get(i)->off();
      }
    }
    return true;
  }
};

#endif /* CHARACTER_DISPLAY_H */
