#include <limits>
#include <WiFi.h>
#include <time.h>
#include <array>
#include <string>

#include "arduino_secrets.h"
#include "hardware.h"
#include "http.h"
#include "network.h"
#include "util.h"

std::array<bool, ZONES> control(const tm& time, long moisture, long flow) {
  std::array<bool, ZONES> active_zones = {};   // All false
  if (time.tm_hour == 7 && time.tm_min >= 45) {
    active_zones[0] = true;
    active_zones[1] = true;
  }
  return active_zones;
}

void setup() {
  Serial.begin(9600);
  setup_network();
  setup_hardware();
  setup_http();
}

void loop() {
  handle_http();
  tm time = {};
  bool time_available = getLocalTime(&time, 10);
  sensor_data sensors = read_sensors();
  std::array<bool, ZONES> active_zones = {};
  if (time_available) active_zones = control(time, sensors.moisture, sensors.flow);
  const http_state& state = get_http_state();
  for (int i = 0; i < ZONES; ++i) {
    active_zones[i] = state.active_zones[i].value_or(active_zones[i]);
  }
  activate_zones(active_zones);
  delay(10);
}
