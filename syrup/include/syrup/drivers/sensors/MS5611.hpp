#ifndef SYRUP_DRIVERS_MS5611_HPP
#define SYRUP_DRIVERS_MS5611_HPP

#include <syrup/drivers/sensors/Sensor.hpp>
#include <stdint.h>
#include <syrup/comm/ByteInterface.hpp>

namespace syrup {
    namespace drivers {
        namespace sensors {
            using communication::ByteInterface;
            class MS5611 : public SuperSensor {
                public:
                    enum {
                        I2C_ADDRESS_HIGH= 0x76,
                        I2C_ADDRESS_LOW = 0x77
                    };
                    enum command {
                        READ            = 0x00,
                        RESET           = 0x1E,
                        CONVERT_D1      = 0x40,
                        CONVERT_D2      = 0x50
                    };
                    enum OSR {
                        OSR256          = 0x00,
                        OSR512          = 0x02,
                        OSR1024         = 0x04,
                        OSR2048         = 0x06,
                        OSR4096         = 0x08,
                    };
                    enum reginfo {
                        PROM_BASE_ADDR  = 0xA2,
                        PROM_REG_COUNT  = 6
                    };
                    uint16_t coeff[];
                    ByteInterface* port;
                    MS5611(ByteInterface* port_, bool = true);
                    void setup();
                    void sample();
                    void timer_sample();
                    void read_PROM();
            };
        }
    }
}
#endif
