#ifndef ANALOG_TELEMETRY_TASK_H
#define ANALOG_TELEMETRY_TASK_H

#include "../AnalogInput/AnalogInput.h"
#include "../MessageSerializer/MessageSerializer.h"
#include "../Task/Task.h"
#include "../util/util.h"

class AnalogTelemetryTask : public Task {
public:
  AnalogTelemetryTask(AnalogInput &input,
                      MessageSerializer &serializer,
                      uint8_t type,
                      unsigned int period_ms,
                      unsigned int decimal_digits) :
      remaining_ms(period_ms),
      seq(0),
      input(input),
      serializer(serializer),
      type(type),
      period_ms(period_ms),
      decimal_digits(decimal_digits) {
  }

  virtual bool run(unsigned int delta_ms) {
    if (remaining_ms > delta_ms) {
      remaining_ms -= delta_ms;
      return true;
    }

    Message<uint32_t> message = {
      type,
      seq,
      float_to_fixed<double, uint32_t>(input.get(), decimal_digits)
    };

    remaining_ms = period_ms;
    ++seq;

    return serializer.write(message);
  }

private:
  unsigned int remaining_ms;
  uint32_t seq;
  const AnalogInput &input;
  MessageSerializer &serializer;
  const uint8_t type;
  const unsigned int period_ms;
  const unsigned int decimal_digits;
};

#endif /* ANALOG_TELEMETRY_TASK_H */
