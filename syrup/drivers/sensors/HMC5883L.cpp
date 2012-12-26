#include <syrup/drivers/sensors/HMC5883L.hpp>
#include <wirish/wirish.h>
#include <syrup/math/math.hpp>
#include <syrup/utils/utils.hpp>

namespace syrup {
    HMC5883L::HMC5883L(ByteInterface* port_, const uint8_t exti_pin_)
    :   SuperSensor(),
        exti_pin(exti_pin_),
        port(port_)
    {
        setup();
        if(exti_pin) {
            pinMode(exti_pin, INPUT_PULLUP);
            attachInterrupt(exti_pin, classInterruptHandler<HMC5883L>, this, RISING);
            port->command(MODE, SINGLE);
        } else {
            port->command(MODE, CONTINUOUS);
        }
    }

    void HMC5883L::setup() {
        port->command(CONFIG_A, AVG8 | HZ75);
        calibrate();
    }

    void HMC5883L::single_sample()
    {
        uint8_t buffer[6];

        port->command(READ);
        port->get(buffer, 6);
        uint16_t d = convert_endian(buffer[0], buffer[1]);
        if(d == 0) return;
        data[bufferswitch][X] += d;
        ++samples[bufferswitch][X];
        data[bufferswitch][Y] += convert_endian(buffer[2], buffer[3]);
        ++samples[bufferswitch][Y];
        data[bufferswitch][Z] += convert_endian(buffer[4], buffer[5]);
        ++samples[bufferswitch][Z];
    }

    void HMC5883L::sample()
    {
        port->command(MODE, SINGLE);
        single_sample();
    }

    void HMC5883L::calibrate() {
    }
} // namespace syrup
