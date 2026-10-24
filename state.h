#ifndef IRRIGATION_STATE_H_
#define IRRIGATION_STATE_H_

#include <array>
#include <optional>
#include <string>

#include "hardware.h"

namespace irrigation {

struct State {
  std::array<std::optional<bool>, kZones> zone_overrides = {};
  std::string control_program;  // Stored only; no interpreter yet.
};

}  // namespace irrigation

#endif  // IRRIGATION_STATE_H_
