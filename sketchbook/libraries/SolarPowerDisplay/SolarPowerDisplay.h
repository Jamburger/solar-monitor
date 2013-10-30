#ifndef SOLAR_POWER_DISPLAY_H
#define SOLAR_POWER_DISPLAY_H

#include "../AnalogInput/AnalogInput.h"
#include "../SevenSegmentDisplay/SevenSegmentDisplay.h"
#include "../SevenSegmentDisplayArray/SevenSegmentDisplayArray.h"
#include "../CharacterDisplay/CharacterDisplay.h"
#include "../DecimalDisplay/DecimalDisplay.h"
#include "../Task/Task.h"

template <size_t Count>
class SolarPowerDisplay : public Task, public SevenSegmentDisplayArray<Count> {
private:
  enum DisplayMode {
    kValue,
    kLabel
  };

public:
  enum Mode {
    kBatteryPotential,
    kSolarCurrent,
    kSolarPotential,
    kSolarPower
  };

  SolarPowerDisplay(SevenSegmentDisplay **displays,
                    AnalogInput &battery_potential,
                    AnalogInput &solar_current,
                    AnalogInput &solar_potential,
                    unsigned int refresh_ms,
                    unsigned int value_ms,
                    unsigned int label_ms) :
      SevenSegmentDisplayArray<Count>(displays),
      decimal_display(displays),
      character_display(displays),
      display_mode(start_display_mode),
      mode(start_mode),
      battery_potential(battery_potential),
      solar_current(solar_current),
      solar_potential(solar_potential),
      refresh_ms(refresh_ms),
      value_ms(value_ms),
      label_ms(label_ms) {
  }

  bool init() {
    return setMode(start_mode);
  }

  virtual bool run(unsigned int delta_ms) {
    // Update the refresh countdown if in value mode.
    if (display_mode == kValue) {
      if (refresh_remaining_ms > delta_ms) {
        refresh_remaining_ms -= delta_ms;
      } else {
        refresh_remaining_ms = refresh_ms;
        refresh();
      }
    }

    // Update the display switch countdown.
    if (display_mode_remaining_ms > delta_ms) {
      display_mode_remaining_ms -= delta_ms;
      return true;
    }

    // Time's up! Change display mode.
    switch (display_mode) {
      case kValue:
        return setDisplayMode(kLabel);

      case kLabel:
        return setDisplayMode(kValue);

      default:
        // Shouldn't get here.
        return false;
    }
  }

  bool setMode(Mode new_mode) {
    mode = new_mode;

    // Note: This display mode persists for at least one cycle.
    return (setDisplayMode(start_display_mode) && refresh());
  }

  bool setDisplayMode(DisplayMode new_mode) {
    display_mode = new_mode;

    switch (new_mode) {
      case kValue:
        display_mode_remaining_ms = value_ms;
        refresh_remaining_ms = refresh_ms;
        break;

      case kLabel:
        display_mode_remaining_ms = label_ms;
        break;

      default:
        // Shouldn't get here.
        return false;
    }

    refresh();

    return true;
  }

private:
  static const DisplayMode start_display_mode = kLabel;
  static const Mode start_mode = kSolarPower;

  bool refresh() {
    switch (display_mode) {
      case kValue:
        return refreshValue();

      case kLabel:
        return refreshLabel();

      default:
        // Shouldn't get here.
        return false;
    }
  }

  bool refreshValue() {
    double value;
    switch (mode) {
      case kBatteryPotential:
        value = battery_potential.get();
        break;

      case kSolarCurrent:
        value = solar_current.get();
        break;

      case kSolarPotential:
        value = solar_potential.get();
        break;

      case kSolarPower:
        value = solar_potential.get() * solar_current.get();
        break;

      default:
        // Shouldn't get here.
        return false;
    }

    return decimal_display.set(value);
  }

  bool refreshLabel() {
    const char * label;
    switch (mode) {
      case kBatteryPotential:
        label = "Vbat";
        break;

      case kSolarCurrent:
        label = "Asol";
        break;

      case kSolarPotential:
        label = "Vsol";
        break;

      case kSolarPower:
        label = "Psol";
        break;

      default:
        // Shouldn't get here.
        return false;
    }

    return character_display.set(label);
  }

  DecimalDisplay<Count> decimal_display;
  CharacterDisplay<Count> character_display;

  DisplayMode display_mode;
  Mode mode;

  unsigned int refresh_remaining_ms;
  unsigned int display_mode_remaining_ms;
  const AnalogInput &battery_potential;
  const AnalogInput &solar_current;
  const AnalogInput &solar_potential;

  const unsigned int refresh_ms;
  const unsigned int value_ms;
  const unsigned int label_ms;
};

#endif
