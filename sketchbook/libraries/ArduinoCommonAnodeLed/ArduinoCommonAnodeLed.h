#ifndef ARDUINO_COMMON_ANODE_LED_H
#define ARDUINO_COMMON_ANODE_LED_H

#include "../ArduinoPin/ArduinoPin.h"
#include "../CommonAnodeLed/CommonAnodeLed.h"

class ArduinoCommonAnodeLed : public CommonAnodeLed {
public:
  ArduinoCommonAnodeLed(unsigned int pin) : CommonAnodeLed(output),
      output(pin) {
  }

  bool init() {
    return output.init();
  }

private:
  ArduinoPin output;
};

#endif /* ARDUINO_COMMON_ANODE_LED_H */
