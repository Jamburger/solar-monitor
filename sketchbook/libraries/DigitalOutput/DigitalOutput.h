#ifndef DIGITAL_OUTPUT_H
#define DIGITAL_OUTPUT_H

class DigitalOutput {
public:
  virtual bool set(bool setting) = 0;
};

#endif
