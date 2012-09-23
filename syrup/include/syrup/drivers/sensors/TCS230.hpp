#ifndef SYRUP_DRIVERS_TCS230_HPP
#define SYRUP_DRIVERS_TCS230_HPP

#include <syrup/drivers/sensors/Sensor.hpp>
#include <stdint.h>
#include <libmaple/timer.h>
#include <wirish/wirish.h>

namespace syrup {
    namespace drivers {
        namespace sensors {
            class TCS230 : public SuperSensor {
                private:
                    uint8_t exti_pin;
                    timer_dev* timer;
                public:
                    TCS230(const uint8_t exti_pin_, HardwareTimer&);
                    void sample();
            };
        }
    }
}
#endif
