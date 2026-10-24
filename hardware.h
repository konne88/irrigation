#ifndef IRRIGATION_HARDWARE_H_
#define IRRIGATION_HARDWARE_H_

#include <array>

namespace irrigation {

inline constexpr int kZones = 8;

void SetupHardware();

struct SensorData {
  long moisture;  // In ohms.
  long flow;
};

SensorData ReadSensors();

void ActivateZones(std::array<bool, kZones> active_zones);

}  // namespace irrigation

#endif  // IRRIGATION_HARDWARE_H_
