#ifndef IRRIGATION_HTTP_H_
#define IRRIGATION_HTTP_H_

#include "settings.h"

namespace irrigation {

// The settings must outlive the registered request handlers.
void SetupHttp(Settings& settings);
void HandleHttp();

}  // namespace irrigation

#endif  // IRRIGATION_HTTP_H_
