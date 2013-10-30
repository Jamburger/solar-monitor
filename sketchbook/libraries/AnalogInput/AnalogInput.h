#ifndef ANALOG_INPUT_H
#define ANALOG_INPUT_H

class AnalogInput {
public:
  virtual double get() const = 0;
};

#endif /* ANALOG_INPUT_H */
