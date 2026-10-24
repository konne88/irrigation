#include <limits>
#include <WiFi.h>
#include <time.h>
#include <array>

#include "arduino_secrets.h"

void setup() {
  Serial.begin(9600);
  setup_network();
  setup_hardware();
}

std::array<bool, ZONES> control(const tm& time, long moisture, long flow) {
  std::array<bool, ZONES> active_zones = {};   // All false
  if (time.tm_hour == 7 && time.tm_min >= 45) {
    active_zones[0] = true;
    active_zones[1] = true;
  }
  return active_zones;
}

struct state {
  std::array<bool, ZONES> overrides; 
  std::string control;
}

void loop() {
  tm time;
  getLocalTime(&time); 
  std::array<bool, ZONES> active_zones = control(time, moisture, flow);

  for (int i = 0; i < ZONES; ++i) {
    if (i > 0) Serial.print(", ");
    Serial.print(active_zones[i] ? 1 : 0);
    digitalWrite(ZONE_PINS[i], active_zones[i] ? HIGH : LOW);
  }
  Serial.println();
  delay(1000);
}
