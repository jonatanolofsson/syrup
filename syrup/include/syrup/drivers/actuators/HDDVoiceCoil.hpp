#ifndef SYRUP_DRIVERS_HDDVOICECOIL_HPP
#define SYRUP_DRIVERS_HDDVOICECOIL_HPP
#include <syrup/drivers/actuators/Actuator.hpp>
#include <syrup/drivers/sensors/TCS230.hpp>
#include <syrup/math/PIDController.hpp>
#include <syrup/utils/Controller.hpp>
#include <stdint.h>

namespace syrup {
    class HDDVoiceCoil : public Actuator {
        public:
            uint16 center;
            int duty_pin;

            HDDVoiceCoil(const int duty_pin_);
            void setControlOutput(const int u);
    };

    typedef Controller<PIDController<int16_t>, TCS230, HDDVoiceCoil> HddControllerType;
    void* HDDidentification(HddControllerType* const that);
    void* measure(HddControllerType* const that);
    void* squareReference(HddControllerType* const that);
}

#endif
