#ifndef AMMETER_ANALOG_INPUT_H
#define AMMETER_ANALOG_INPUT_H

#include "../AnalogInput/AnalogInput.h"

class AmmeterAnalogInput : public AnalogInput {
public:
  AmmeterAnalogInput(AnalogInput &volts_input, double resistance) :
      volts_input(volts_input), resistance(resistance) {
  }

  virtual double get() const {
    double volts = volts_input.get();
    return volts / resistance;
  }

private:
  const AnalogInput &volts_input;
  const double resistance;
};

#endif /* AMMETER_ANALOG_INPUT_H */
