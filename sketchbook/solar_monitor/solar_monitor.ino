#include <DecimalDisplay.h>
#include <AmmeterAnalogInput.h>
#include <AnalogTelemetryTask.h>
#include <MessageSerializer.h>
#include <Message.h>
#include <ArduinoCommonAnodeLed.h>
#include <ArduinoAnalogInput.h>
#include <AnalogInputMultiplier.h>
#include <DigitalOutputSevenSegmentDisplay.h>
#include <SevenSegmentDisplay.h>
#include <SolarPowerDisplay.h>
#include <SolarPowerDisplayTask.h>

#include <stdint.h>

// Serial baud rate.
static const unsigned int serial_baud = 57600;

// Dummy pin; not attached to anything.
static const unsigned int dummy_pin = 22;

// ADC internal 1.1V, 2.56V reference errors.
const double adc_1v1_offset_error = -.0027303;
const double adc_1v1_gain_error = 1.04659;
const double adc_2v56_offset_error = 0.028929;
const double adc_2v56_gain_error = 1.017857;

// Analog input for battery potential measurement.
static const unsigned int battery_potential_pin = A1;

// Analog reference for battery potential measurement.
static const ArduinoAnalogInput::Reference battery_potential_reference =
    ArduinoAnalogInput::k2V56;

// The voltage drop across the battery is split across a voltage divider.
// The measured potential is across a piece of the divider. The measured
// value is scaled by this amount to compensate. In our case, we have two
// 100 kOhm resistors. Empirically, the scalar was found to be 1.9919.
static const double battery_potential_divider_scalar = 1.9919;

// Analog input for solar current measurement.
static const unsigned int solar_current_pin = A0;

// Analog reference for solar current measurement.
static const ArduinoAnalogInput::Reference solar_current_reference =
    ArduinoAnalogInput::k1V1;

// Analog input for solar potential measurement.
static const unsigned int solar_potential_pin = A15;

// Analog reference for solar potential measurement.
static const ArduinoAnalogInput::Reference solar_potential_reference =
    ArduinoAnalogInput::k1V1;

// Solar current meter resistance. This resistance is in series with the solar
// panel. The voltage drop across this resistor yields the current.
static const double solar_current_meter_resistance = 1.0;

// The voltage drop across the solar panel is split across a voltage divider.
// The measured potential is across a piece of the divider. The measured value
// is scaled by this amount to compensate. In our case, we have five 2 MOhm
// resistors. Empirically, the scalar was found to be 5.1626.
static const double solar_potential_divider_scalar = 5.1626;

// Telemetry ID for battery potential measurements.
static const uint8_t battery_potential_telemetry_type = 0;

// Telemetry ID for solar current measurements.
static const uint8_t solar_current_telemetry_type = 1;

// Telemetry ID for slar potential measurements.
static const uint8_t solar_potential_telemetry_type = 2;

// Battery potential telemetry period, in milliseconds.
static const unsigned int battery_potential_telemetry_period = 1000;

// Solar current telemetry period, in milliseconds.
static const unsigned int solar_current_telemetry_period = 1000;

// Solar potential telemetry period, in milliseconds.
static const unsigned int solar_potential_telemetry_period = 1000;

// Fixed-point post-decimal-point digits to use in telemetry
static const unsigned int telemetry_decimal_digits = 3;

// Battery potential input. The potential is measured across ~1/2 of a
// high-resistance voltage divider.
static ArduinoAnalogInput battery_potential_divided(
    battery_potential_pin,
    battery_potential_reference);
static AnalogInputMultiplier battery_potential(
    battery_potential_divided,
    battery_potential_divider_scalar);

// Solar current input. The current is measured by applying Ohm's Law across
// an in-series resistor.
static ArduinoAnalogInput solar_current_divider_volts(
    solar_current_pin,
    solar_current_reference);
static AmmeterAnalogInput solar_current(
    solar_current_divider_volts,
    solar_current_meter_resistance);

// Solar potential input. The potential is measured across ~1/5 of a
// high-resistance voltage divider.
static ArduinoAnalogInput solar_potential_divided(
    solar_potential_pin,
    solar_potential_reference);
static AnalogInputMultiplier solar_potential(
    solar_potential_divided,
    solar_potential_divider_scalar);

// Make an array of the 4 available seven-segment displays.
static DigitalOutputSevenSegmentDisplay<ArduinoCommonAnodeLed> display1(
    53, A14, 14, 15, 16, 51, 49, 2);
static DigitalOutputSevenSegmentDisplay<ArduinoCommonAnodeLed> display2(
    A10, A11, 5, 4, 3, A12, A13, 6);
static DigitalOutputSevenSegmentDisplay<ArduinoCommonAnodeLed> display3(
    A7, A6, 9, 8, 7, A8, A9, 10);
static DigitalOutputSevenSegmentDisplay<ArduinoCommonAnodeLed> display4(
    A3, A2, 13, 12, 11, A4, A5, dummy_pin);
static SevenSegmentDisplay *displays[] = {
  &display1,
  &display2,
  &display3,
  &display4
};

// Create a seven-segment display manager.
static SolarPowerDisplay<4> solar_power_display(
    displays,
    battery_potential,
    solar_current,
    solar_potential,
    500,
    6500,
    1500);

static SolarPowerDisplayTask<4> solar_power_display_task(
    solar_power_display,
    4000,
    4000,
    4000,
    4000);

// Create a message serializer. The serializer stringifies datagrams for
// transmission over a stream-like communication link.
static MessageSerializer telemetry_serializer(Serial);

// Create telemetry tasks.j
static AnalogTelemetryTask battery_potential_telemetry(
    battery_potential,
    telemetry_serializer,
    battery_potential_telemetry_type,
    battery_potential_telemetry_period,
    telemetry_decimal_digits);

static AnalogTelemetryTask solar_current_telemetry(solar_current,
    telemetry_serializer,
    solar_current_telemetry_type,
    solar_current_telemetry_period,
    telemetry_decimal_digits);

static AnalogTelemetryTask solar_potential_telemetry(solar_potential,
    telemetry_serializer,
    solar_potential_telemetry_type,
    solar_potential_telemetry_period,
    telemetry_decimal_digits);

static bool initialized = false;

void setup() {
  if (!display1.init() || !display2.init() || !display3.init() || !display4.init() ||
      !solar_power_display.init() || !solar_power_display_task.init()) {
    return;
  }
  
  // Set up ADC errors.
  ArduinoAnalogInput::setOffsetError(ArduinoAnalogInput::k1V1, adc_1v1_offset_error);
  ArduinoAnalogInput::setGainError(ArduinoAnalogInput::k1V1, adc_1v1_gain_error);
  ArduinoAnalogInput::setOffsetError(ArduinoAnalogInput::k2V56, adc_2v56_offset_error);
  ArduinoAnalogInput::setGainError(ArduinoAnalogInput::k2V56, adc_2v56_gain_error);
  
  Serial.begin(serial_baud);
  
  initialized = true;
}

void loop() {
  static unsigned long delta = 0;
  unsigned long start = millis();

  // Update displays.
  solar_power_display.run(delta);
  solar_power_display_task.run(delta);

  // Update telemetry.
  battery_potential_telemetry.run(delta);
  solar_current_telemetry.run(delta);
  solar_potential_telemetry.run(delta);

  // Hiccup the CPU.
  delay(10);

  delta = millis() - start;
}
