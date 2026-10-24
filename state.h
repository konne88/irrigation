#ifndef IRRIGATION_STATE_H_
#define IRRIGATION_STATE_H_

#include <time.h>

#include <array>
#include <optional>
#include <string>

#include "hardware.h"

namespace irrigation {

struct State {
  std::array<std::optional<bool>, kZones> zone_overrides = {};
  time_t zone_overrides_end = 0;  // Unix seconds; zero means inactive.
  std::string control_program;  // Stored only; no interpreter yet.
};

}  // namespace irrigation

#endif  // IRRIGATION_STATE_H_
