#ifndef SOLAR_POWER_DISPLAY_TASK_H
#define SOLAR_POWER_DISPLAY_TASK_H

#include "../Task/Task.h"
#include "../SolarPowerDisplay/SolarPowerDisplay.h"

template <size_t Count>
class SolarPowerDisplayTask : public Task {
private:
  typedef SolarPowerDisplay<Count> SolarPowerDisplayType;

  static const typename SolarPowerDisplayType::Mode start_mode =
      SolarPowerDisplayType::kBatteryPotential;
  static const int mode_count = 4;

public:
  SolarPowerDisplayTask(SolarPowerDisplayType &display,
                        unsigned int battery_potential_ms,
                        unsigned int solar_current_ms,
                        unsigned int solar_potential_ms,
                        unsigned int solar_power_ms) :
      display(display),
      mode(start_mode),
      ms_remaining(battery_potential_ms),
      battery_potential_ms(battery_potential_ms),
      solar_current_ms(solar_current_ms),
      solar_potential_ms(solar_potential_ms),
      solar_power_ms(solar_power_ms) {
  }

  bool init() {
    return (display.init() && display.setMode(mode));
  }

  bool run(unsigned int ms_delta) {
    if (ms_remaining > ms_delta) {
      ms_remaining -= ms_delta;
      return true;
    }

    ms_remaining = 0;

    // Time's up!
    // Find a new mode, skipping modes that have no time associated with them.
    // To avoid infinite loops, give up after all states have been considered.
    for (int i = 0; i < mode_count && ms_remaining == 0; ++i) {
      switch (mode) {
        case SolarPowerDisplayType::kBatteryPotential:
          mode = SolarPowerDisplayType::kSolarCurrent;
          ms_remaining = solar_current_ms;
          break;

        case SolarPowerDisplayType::kSolarCurrent:
          mode = SolarPowerDisplayType::kSolarPotential;
          ms_remaining = solar_potential_ms;
          break;

        case SolarPowerDisplayType::kSolarPotential:
          mode = SolarPowerDisplayType::kSolarPower;
          ms_remaining = solar_power_ms;
          break;

        case SolarPowerDisplayType::kSolarPower:
          mode = SolarPowerDisplayType::kBatteryPotential;
          ms_remaining = battery_potential_ms;
          break;
      }
    }

    return display.setMode(mode);
  }

private:
  SolarPowerDisplayType &display;
  enum SolarPowerDisplayType::Mode mode;

  unsigned int ms_remaining;
  unsigned int battery_potential_ms;
  unsigned int solar_current_ms;
  unsigned int solar_potential_ms;
  unsigned int solar_power_ms;
};

#endif /* SOLAR_POWER_DISPLAY_TASK_H */
