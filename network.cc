#include "network.h"

#include <time.h>

#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFi.h>

#include "arduino_secrets.h"

namespace irrigation {

void SetupNetwork() {
  // Set up Wi-Fi.
  WiFi.begin(kWifiSsid, kWifiPassword);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Mac: ");
  Serial.println(WiFi.macAddress());

  // Resolve garden.local to this device on the local network.
  if (!MDNS.begin("garden")) {
    Serial.println("Failed to start mDNS; garden.local will be unavailable.");
  }

  // Configure NTP and the Pacific time zone.
  configTzTime("PST8PDT,M3.2.0,M11.1.0", "pool.ntp.org");
}

}  // namespace irrigation
