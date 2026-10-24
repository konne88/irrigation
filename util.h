#ifndef IRRIGATION_UTIL_H_
#define IRRIGATION_UTIL_H_

#include <Arduino.h>

// Evaluate once and report failures to Serial; execution continues.
#define IRRIGATION_CHECK(command)                                             \
  do {                                                                        \
    if (!(command)) {                                                         \
      Serial.print("CHECK failed: " #command " at ");                         \
      Serial.print(__FILE__);                                                 \
      Serial.print(':');                                                      \
      Serial.println(__LINE__);                                               \
    }                                                                         \
  } while (false)

#endif  // IRRIGATION_UTIL_H_
