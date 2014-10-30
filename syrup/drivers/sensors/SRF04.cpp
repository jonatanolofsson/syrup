#include <syrup/drivers/sensors/SRF04.hpp>
#include <wirish/wirish.h>
#include <syrup/math/math.hpp>
#include <stdint.h>

namespace syrup {
    SRF04::SRF04(const uint8_t exti_pin_, HardwareTimer& timer_, const bool master_)
    :   SuperSensor(),
        exti_pin(exti_pin_),
        timer(timer_.c_dev()),
        master(master_)
    {
        pinMode(exti_pin, INPUT);
        timer_.setPeriod(3000);
        attachInterrupt(exti_pin, classInterruptHandler<SRF04>, this, (master ? CHANGE : FALLING));
    }

    void SRF04::sample()
    {
        if(!master || !digitalRead(exti_pin)) {
            data[0] += timer_get_count(timer);
            ++samples;
        } else {
            if(master) {
                timer_generate_update(timer);
            }
        }
    }
} // namespace syrup
