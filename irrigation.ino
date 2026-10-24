#include <time.h>

#include <array>

#include <Arduino.h>

#include "control.h"
#include "hardware.h"
#include "http.h"
#include "network.h"
#include "state.h"

namespace {

irrigation::State state;

}  // namespace

void setup() {
  Serial.begin(9600);
  irrigation::SetupHardware();
  irrigation::SetupNetwork();
  irrigation::SetupHttp(state);
  Serial.println("Irrigation controller ready at http://garden.local");
}

void loop() {
  irrigation::HandleHttp();
  tm time = {};
  bool time_available = getLocalTime(&time, 10);
  irrigation::SensorData sensors = irrigation::ReadSensors();
  std::array<bool, irrigation::kZones> active_zones = {};
  if (time_available) {
    active_zones = irrigation::Control(state.control_program, time,
                                      sensors.moisture, sensors.flow);
    if (::time(nullptr) < state.zone_overrides_end) {
      for (int i = 0; i < irrigation::kZones; ++i) {
        active_zones[i] = state.zone_overrides[i].value_or(active_zones[i]);
      }
    }
  }
  irrigation::ActivateZones(active_zones);
  delay(10);
}
