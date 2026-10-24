#ifndef IRRIGATION_CONTROL_H_
#define IRRIGATION_CONTROL_H_

#include <time.h>

#include <array>
#include <string>

#include "hardware.h"

namespace irrigation {

// Run control script with the given context. Errors leave every zone off.
std::array<bool, kZones> Control(const std::string& program, const tm& time,
                                 long moisture, long flow);

}  // namespace irrigation

#endif  // IRRIGATION_CONTROL_H_
