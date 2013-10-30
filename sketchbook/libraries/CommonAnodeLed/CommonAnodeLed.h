#ifndef COMMON_ANODE_LED
#define COMMON_ANODE_LED

#include "../DigitalOutput/DigitalOutput.h"

class CommonAnodeLed : public DigitalOutput {
public:
  CommonAnodeLed(DigitalOutput &output) : output(output) {
  }

  virtual bool set(bool setting) {
    // Negation for common anode.
    return output.set(!setting);
  }

private:
  DigitalOutput &output;
};

#endif /* COMMON_ANODE_LED */
