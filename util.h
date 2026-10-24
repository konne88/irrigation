#pragma once

#include <Arduino.h>

// Evaluate once and report failures to Serial; execution continues.
#define CHECK(command)                              \
  do {                                              \
    if (!(command)) {                               \
      Serial.print("CHECK failed: " #command " at "); \
      Serial.print(__FILE__);                        \
      Serial.print(':');                             \
      Serial.println(__LINE__);                      \
    }                                               \
  } while (false)
