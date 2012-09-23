#ifndef SYRUP_DRIVERS_HMC5883L_HPP
#define SYRUP_DRIVERS_HMC5883L_HPP

#include <syrup/drivers/sensors/Sensor.hpp>
#include <stdint.h>
#include <syrup/comm/ByteInterface.hpp>

namespace syrup {
    namespace drivers {
        namespace sensors {
            using communication::ByteInterface;
            class HMC5883L : public SuperSensor {
                private:
                    uint8_t exti_pin;
                public:
                    enum address {
                        I2C_ADDRESS     = 0x1E
                    };
                    enum registers {
                        CONFIG_A        = 0x00,
                        CONFIG_B        = 0x01,
                        MODE            = 0x02,
                        READ            = 0x03,
                    };
                    enum bit_offset {
                        MA              = 0x05,
                        DO              = 0x03,
                        MS              = 0x00
                    };
                    enum sample_average {
                        AVG1            = 0x00 << MA,
                        AVG2            = 0x01 << MA,
                        AVG4            = 0x02 << MA,
                        AVG8            = 0x03 << MA
                    };
                    enum output_rate {
                        HZ0_75          = 0x00 << DO,
                        HZ1_5           = 0x01 << DO,
                        HZ3             = 0x02 << DO,
                        HZ7_5           = 0x03 << DO,
                        HZ15            = 0x04 << DO,
                        HZ30            = 0x05 << DO,
                        HZ75            = 0x06 << DO
                    };
                    enum measure_types {
                        NORMAL          = 0x00,
                        POS_BIAS        = 0x02,
                        NEG_BIAS        = 0x03
                    };
                    enum modes {
                        CONTINUOUS      = 0x00,
                        SINGLE          = 0x01,
                        IDLE            = 0x02
                    };
                    enum axes {
                        X = 0,
                        Y = 1,
                        Z = 2
                    };
                    ByteInterface* port;
                    HMC5883L(ByteInterface* port_, const uint8_t exti_pin_ = 0);
                    void setup();
                    void single_sample();
                    void sample();
                    void calibrate();
            };
        }
    }
}
#endif
