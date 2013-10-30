#ifndef PIN_H
#define PIN_H

#include "../DigitalOutput/DigitalOutput.h"

#include "Arduino.h"

class ArduinoPin : public DigitalOutput {
public:
  ArduinoPin(unsigned int arduino_pin) : arduino_pin(arduino_pin) {
  }

  bool init() {
    pinMode(arduino_pin, OUTPUT);
    return true;
  }

  virtual bool set(bool setting) {
    digitalWrite(arduino_pin, setting ? HIGH : LOW);
    return true;
  }

private:
  unsigned int arduino_pin;
};

#endif
