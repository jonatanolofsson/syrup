#include <syrup/drivers/sensors/TCS230.hpp>
#include <wirish/wirish.h>
#include <syrup/math/math.hpp>
#include <stdint.h>
#include <syrup/isr.hpp>

namespace syrup {
    TCS230::TCS230(const uint8_t exti_pin_, HardwareTimer& timer_)
    :   SuperSensor(),
        exti_pin(exti_pin_),
        timer(timer_.c_dev())
    {
        pinMode(exti_pin, INPUT);
        timer_.setPrescaleFactor(1);
        timer_.setOverflow(0xffff);
        timer_.refresh();
        attachInterrupt(exti_pin, &isr::invokeMember<TCS230>, this, CHANGE);
    }

    void TCS230::sample()
    {
        data[0] += timer_get_count(timer);
        ++samples;
        timer_generate_update(timer);
    }
} // namespace syrup
