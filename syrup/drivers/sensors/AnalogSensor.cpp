#include <syrup/drivers/sensors/AnalogSensor.hpp>
#include <wirish/wirish.h>

namespace syrup {
    AnalogSensor::AnalogSensor(const int pin_)
    :   SuperSensor<1>(),
        pin(pin_)
    {
        setup();
    }
    void AnalogSensor::setup() {
        pinMode(pin, INPUT);
    }
    void AnalogSensor::sample() {
        data[bufferswitch][0] += analogRead(pin);
        ++samples[bufferswitch][0];
    }
}
