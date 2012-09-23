#include <syrup/drivers/sensors/TCS230.hpp>
#include <wirish/wirish.h>
#include <syrup/math/math.hpp>
#include <stdint.h>
#include <syrup/utils/utils.hpp>

namespace syrup {
    namespace drivers {
        namespace sensors {
            TCS230::TCS230(const uint8_t exti_pin_, HardwareTimer& timer_)
            :   SuperSensor(1),
                exti_pin(exti_pin_),
                timer(timer_.c_dev())
            {
                pinMode(exti_pin, INPUT);
                timer_.setPrescaleFactor(1);
                timer_.setOverflow(0xffff);
                timer_.refresh();
                attachInterrupt(exti_pin, utils::classInterruptHandler<TCS230>, this, CHANGE);
            }

            void TCS230::sample()
            {
                bool mbuffer = bufferswitch;
                data[mbuffer][0] += timer_get_count(timer);
                ++samples[mbuffer][0];
                timer_generate_update(timer);
            }
        } // namespace sensors
    } // namespace drivers
} // namespace syrup
