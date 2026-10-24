#ifndef IRRIGATION_SETTINGS_H_
#define IRRIGATION_SETTINGS_H_

#include <time.h>

#include <array>
#include <optional>
#include <string>

#include "hardware.h"

namespace irrigation {

struct Settings {
  std::array<std::optional<bool>, kZones> zone_overrides = {};
  time_t zone_overrides_end = 0;  // Unix seconds; zero means inactive.
  std::string control_program =
      "if hour == 7 and minute >= 45 then\n"
      "    active_zones[1] = true\n"
      "    active_zones[2] = true\n"
      "end\n";
};

}  // namespace irrigation

#endif  // IRRIGATION_SETTINGS_H_
