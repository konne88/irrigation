#pragma once

#include <Arduino.h>
#include <array>
#include <optional>

#include "hardware.h"

struct http_state {
  std::array<std::optional<bool>, ZONES> active_zones = {};
  String control_program;  // Stored only; no interpreter yet.
};

void setup_http();
void handle_http();
const http_state& get_http_state();
