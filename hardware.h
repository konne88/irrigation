void setup_hardware();

struct sensor_data {
    long moisture;  // in ohm
};

sensor_data read_sensors();