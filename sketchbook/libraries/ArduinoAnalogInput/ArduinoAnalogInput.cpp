#include "ArduinoAnalogInput.h"

// The offset errors for each of the analog references. This array size must be
// kept consistent with ArduinoAnalogInput::kReferenceCount.
double ArduinoAnalogInput::offset_errors[] = { 0, 0, 0 };

// The gain errors for each of the analog references. This array size must be
// kept consistent with ArduinoAnalogInput::kReferenceCount.
double ArduinoAnalogInput::gain_errors[] = {1, 1, 1 };
