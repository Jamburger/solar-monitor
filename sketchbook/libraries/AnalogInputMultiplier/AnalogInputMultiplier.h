#ifndef ANALOG_INPUT_MULTIPLIER_H
#define ANALOG_INPUT_MULTIPLIER_H

#include "../AnalogInput/AnalogInput.h"

class AnalogInputMultiplier : public AnalogInput {
public:
  AnalogInputMultiplier(AnalogInput &input, double multiplier) :
      input(input), multiplier(multiplier) {
  }

  double get() const {
    return input.get() * multiplier;
  }

private:
  AnalogInput &input;
  double multiplier;
};

#endif  /* ANALOG_INPUT_MULTIPLIER_H */
