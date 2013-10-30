#ifndef ARDUINO_ANALOG_INPUT_H
#define ARDUINO_ANALOG_INPUT_H

#include "../AnalogInput/AnalogInput.h"

#include "Arduino.h"

#include "HardwareSerial.h"

class ArduinoAnalogInput : public AnalogInput {
public:
  enum Reference {
    kDefault,
    k1V1,
    k2V56
  };

  ArduinoAnalogInput(unsigned int pin,
                    Reference reference,
                    double offset_error,
                    double gain_error) :
      pin(pin),
      arduino_reference(getArduinoReference(reference)),
      voltage_reference(getVoltageReference(reference)),
      offset_error(offset_error),
      gain_error(gain_error) {
  }

  bool init() {
    pinMode(pin, INPUT);
    return true;
  }

  double get() const {
    analogReference(arduino_reference);

    // Read twice, as recommended by the data sheet. The first reading following
    // a mux switch or reference voltage switch is unreliable.
    analogRead(pin);

    // Wait for stuff to stabilize. If this isn't here, the first measurement
    // is often quite wrong. This sucks, but hey, Arduino.
    delay(5);

    // Convert to voltage and adjust for errors.
    const unsigned int raw_lsbs = analogRead(pin);
    const double raw_voltage = ((double) raw_lsbs * voltage_reference) / 1024;

    // Note that if there was a negative offset error, it is not possible for
    // this value to ever achieve zero.
    return (raw_voltage - offset_error) / gain_error;
  }

private:
  static int getArduinoReference(Reference reference) {
    switch (reference) {
      case kDefault:
        return DEFAULT;

      case k1V1:
        return INTERNAL1V1;

      case k2V56:
        return INTERNAL2V56;

      default:
        // Should never get here.
        return DEFAULT;
    }
  }

  static double getVoltageReference(Reference reference) {
    switch (reference) {
      case kDefault:
        return 5.0;

      case k1V1:
        return 1.1;

      case k2V56:
        return 2.56;

      default:
        // Should never get here.
        return 5.0;
    }
  }

  unsigned int pin;
  int arduino_reference;
  double voltage_reference;

  /// The voltage error in volts.
  const double offset_error;

  /// The gain error scalar.
  const double gain_error;
};

#endif /* ARDUINO_ANALOG_INPUT_H */
