#ifndef CYCLING_DISPLAY_H
#define CYCLING_DISPLAY_H

#include "../CharacterDisplay/CharacterDisplay.h"
#include "../DecimalDisplay/DecimalDisplay.h"
#include "../SevenSegmentDisplay/SevenSegmentDisplay.h"
#include "../SevenSegmentDisplayArray/SevenSegmentDisplayArray.h"
#include "../Task/Task.h"

template <size_t Count>
class CyclingDisplay : public Task, public SevenSegmentDisplayArray<Count> {
private:
  
};

#endif /* CYCLING_DISPLAY_H */
