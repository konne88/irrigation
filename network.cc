#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <time.h>

#include "arduino_secrets.h"
#include "network.h"
#include "util.h"

void setup_network() {
  // Setup wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
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
  CHECK(MDNS.begin("garden"));

   // Configure NTP and Pacific timezone.
  configTzTime("PST8PDT,M3.2.0,M11.1.0", "pool.ntp.org");
}
