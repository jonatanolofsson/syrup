#ifndef SYRUP_DRIVERS_SENSORS_RANGE_SENSOR_HPP_
#define SYRUP_DRIVERS_SENSORS_RANGE_SENSOR_HPP_
#include <syrup/drivers/sensors/AnalogSensor.hpp>

namespace syrup {
    class RangeSensor : public AnalogSensor {
        public:
            RangeSensor(int pin) : AnalogSensor(pin) {}
    };
}

#endif
