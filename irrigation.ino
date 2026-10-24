#include <limits>
#include <WiFi.h>
#include <time.h>
#include <array>
#include <string>

#include "arduino_secrets.h"
#include "hardware.h"
#include "network.h"

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
}

void loop() {
  tm time;
  getLocalTime(&time); 
  sensor_data sensors = read_sensors();
  std::array<bool, ZONES> active_zones = control(time, sensors.moisture, sensors.flow);
  activate_zones(active_zones);
  delay(1000);
}
