#ifndef SYRUP_DRIVERS_SENSORS_ANALOG_SENSOR_HPP_
#define SYRUP_DRIVERS_SENSORS_ANALOG_SENSOR_HPP_
#include <syrup/drivers/sensors/Sensor.hpp>

namespace syrup {
    class AnalogSensor : public SuperSensor<1> {
        public:
            int pin;
            AnalogSensor(const int pin_);
            void setup();
            void sample();
    };
}

#endif
