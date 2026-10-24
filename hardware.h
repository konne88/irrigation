#pragma once

#include <Arduino.h>
#include <array>

const int ZONES = 8;

void setup_hardware();

struct sensor_data {
    long moisture;  // in ohm
    long flow;
};

sensor_data read_sensors();

void activate_zones(std::array<bool, ZONES> active_zones);
