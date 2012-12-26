#ifndef SYRUP_DRIVERS_SRF04_HPP
#define SYRUP_DRIVERS_SRF04_HPP

#include <syrup/drivers/sensors/Sensor.hpp>
#include <stdint.h>
#include <libmaple/timer.h>
#include <wirish/wirish.h>

namespace syrup {
    class SRF04 : public SuperSensor<1> {
        private:
            uint8_t exti_pin;
            timer_dev* timer;
            bool master;
        public:
            SRF04(const uint8_t exti_pin_, HardwareTimer& timer_, const bool master_ = false);
            void sample();
    };
}
#endif
