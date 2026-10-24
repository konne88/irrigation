#ifndef IRRIGATION_HTTP_H_
#define IRRIGATION_HTTP_H_

#include "state.h"

namespace irrigation {

// The state must outlive the registered request handlers.
void SetupHttp(State& state);
void HandleHttp();

}  // namespace irrigation

#endif  // IRRIGATION_HTTP_H_
