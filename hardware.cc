const int MOISTURE_PIN_A = A7;
const int MOISTURE_PIN_READ = A6;
const int MOISTURE_PIN_B = A5;
const long MOISTURE_FIXED_RESISTANCE = 10000; // in ohm

const int FLOW_PIN = D10;

const int ZONES = 8;
const int ZONE_PINS[] = {
  D2, D3, D4, D5, D6, D7, D8, D9
};

volatile unsigned long flowPulseCount = 0;

void IRAM_ATTR onFlowPulse() {
  ++flowPulseCount;
}

void setup_hardware() {
  // Setup flow sensor.
  pinMode(FLOW_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(FLOW_PIN),
                  onFlowPulse, FALLING);
  analogReadResolution(12);  // 12 bit read resolution

  // Setup zones.
  for (int i = 0; i < ZONES; ++i) {
    digitalWrite(ZONE_PINS[i], LOW); 
    pinMode(ZONE_PINS[i], OUTPUT);
  }
}

long moisture_last_reading = LONG_MIN;
long moisture = 0;

// Returns the moisture resistance in ohm.
long read_moisture() {
  // Only read every 10 minutes;
  if (millis() - moisture_last_reading > 600000) {

    // Connect electrodes to perform measurement.
    // A --> [moisture] --> READ --> [ MOISTURE_FIXED_RESISTANCE ] --> B
    digitalWrite(MOISTURE_PIN_A, HIGH); 
    digitalWrite(MOISTURE_PIN_B, LOW);
    pinMode(MOISTURE_PIN_A, OUTPUT);
    pinMode(MOISTURE_PIN_B, OUTPUT);
    delay(1);
    long voltage = analogReadMilliVolts(MOISTURE_PIN_READ);

    // Send voltage in the other direction, to avoid corrosion.
    digitalWrite(MOISTURE_PIN_A, LOW); 
    digitalWrite(MOISTURE_PIN_B, HIGH);
    delay(1);

    // Disconnect electrodes after measurement.
    pinMode(MOISTURE_PIN_A, INPUT);
    pinMode(MOISTURE_PIN_B, INPUT);

    if (voltage == 0) return LONG_MAX;

    moisture = (3300 * MOISTURE_FIXED_RESISTANCE / voltage) - MOISTURE_FIXED_RESISTANCE;
    moisture_last_reading = millis();
  }

  return moisture;
}

long read_flow() {
  noInterrupts();
  unsigned long count = flowPulseCount;
  interrupts();
  return count;
}

sensor_data read_sensors() {
  long moisture = read_moisture();

  return {
    .moisture = moisture
  }
}



