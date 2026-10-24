#include "hardware.h"

#include <climits>

#include <Arduino.h>

namespace irrigation {
namespace {

constexpr int kMoisturePinA = A7;
constexpr int kMoisturePinRead = A6;
constexpr int kMoisturePinB = A5;
constexpr long kMoistureFixedResistance = 10000;  // In ohms.
constexpr int kFlowPin = D10;
constexpr int kZonePins[] = {D2, D3, D4, D5, D6, D7, D8, D9};

volatile unsigned long flow_pulse_count = 0;
long moisture_last_reading = LONG_MIN;
long moisture = 0;

void IRAM_ATTR OnFlowPulse() { ++flow_pulse_count; }

// Returns the moisture resistance in ohms.
long ReadMoisture() {
  // Only read every 10 minutes.
  if (millis() - moisture_last_reading > 600000) {
    // Connect electrodes to perform measurement.
    // A --> [moisture] --> READ --> [kMoistureFixedResistance] --> B
    digitalWrite(kMoisturePinA, HIGH);
    digitalWrite(kMoisturePinB, LOW);
    pinMode(kMoisturePinA, OUTPUT);
    pinMode(kMoisturePinB, OUTPUT);
    delay(1);
    long voltage = analogReadMilliVolts(kMoisturePinRead);

    // Send voltage in the other direction to avoid corrosion.
    digitalWrite(kMoisturePinA, LOW);
    digitalWrite(kMoisturePinB, HIGH);
    delay(1);

    // Disconnect electrodes after measurement.
    pinMode(kMoisturePinA, INPUT);
    pinMode(kMoisturePinB, INPUT);

    if (voltage == 0) {
      return LONG_MAX;
    }

    moisture = (3300 * kMoistureFixedResistance / voltage) -
               kMoistureFixedResistance;
    moisture_last_reading = millis();
  }

  return moisture;
}

long ReadFlow() {
  noInterrupts();
  unsigned long count = flow_pulse_count;
  interrupts();
  return count;
}

}  // namespace

void SetupHardware() {
  // Set up the flow sensor.
  pinMode(kFlowPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(kFlowPin), OnFlowPulse, FALLING);
  analogReadResolution(12);  // 12-bit read resolution.

  // Set up the zones.
  for (int i = 0; i < kZones; ++i) {
    digitalWrite(kZonePins[i], LOW);
    pinMode(kZonePins[i], OUTPUT);
  }
}

SensorData ReadSensors() { return {ReadMoisture(), ReadFlow()}; }

void ActivateZones(std::array<bool, kZones> active_zones) {
  for (int i = 0; i < kZones; ++i) {
    digitalWrite(kZonePins[i], active_zones[i] ? HIGH : LOW);
  }
}

}  // namespace irrigation
