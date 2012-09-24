#ifndef SYRUP_DRIVERS_HDDVOICECOIL_HPP
#define SYRUP_DRIVERS_HDDVOICECOIL_HPP
#include <syrup/drivers/actuators/Actuator.hpp>
#include <syrup/drivers/sensors/TCS230.hpp>
#include <syrup/math/PIDController.hpp>
#include <syrup/utils/Controller.hpp>
#include <stdint.h>

namespace syrup {
    namespace drivers {
        namespace actuators {
            class HDDVoiceCoil : public Actuator {
                public:
                    //~ uint16 power0;
                    uint16 center;
                    int pwr_pin;
                    int duty_pin;

                    HDDVoiceCoil(const int pwr_pin_, const int duty_pin_);
                    void setControlOutput(const int u);
            };

            typedef utils::Controller<math::PIDController<int16_t>, sensors::TCS230, HDDVoiceCoil> HddControllerType;
            void* HDDidentification(HddControllerType* const that);
        }
    }
}

#endif
