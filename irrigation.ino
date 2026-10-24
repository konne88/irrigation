#include <time.h>

#include <array>

#include <Arduino.h>
#include <WiFi.h>

#include "hardware.h"
#include "http.h"
#include "network.h"
#include "state.h"

namespace {

irrigation::State state;

std::array<bool, irrigation::kZones> Control(const tm& time, long moisture,
                                             long flow) {
  std::array<bool, irrigation::kZones> active_zones = {};  // All false.
  if (time.tm_hour == 7 && time.tm_min >= 45) {
    active_zones[0] = true;
    active_zones[1] = true;
  }
  return active_zones;
}

}  // namespace

void setup() {
  Serial.begin(9600);
  irrigation::SetupNetwork();
  irrigation::SetupHardware();
  irrigation::SetupHttp(state);
}

void loop() {
  irrigation::HandleHttp();
  tm time = {};
  bool time_available = getLocalTime(&time, 10);
  irrigation::SensorData sensors = irrigation::ReadSensors();
  std::array<bool, irrigation::kZones> active_zones = {};
  if (time_available) {
    active_zones = Control(time, sensors.moisture, sensors.flow);
  }
  for (int i = 0; i < irrigation::kZones; ++i) {
    active_zones[i] = state.zone_overrides[i].value_or(active_zones[i]);
  }
  irrigation::ActivateZones(active_zones);
  delay(10);
}
