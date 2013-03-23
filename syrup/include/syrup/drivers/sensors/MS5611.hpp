#ifndef SYRUP_DRIVERS_MS5611_HPP
#define SYRUP_DRIVERS_MS5611_HPP

#include <syrup/drivers/sensors/Sensor.hpp>
#include <stdint.h>
#include <libmaple/i2c.h>

namespace syrup {
    class MS5611 : public SuperSensor<1> {
        private:
            i2c_dev* device;

            uint8_t buffer[3];
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
            //~ uint16_t coeff[];
            MS5611(i2c_dev* const dev_);
            void setup();
            void sample();
            void saveData(struct i2c_msg*);
            void read_PROM();
    };
}
#endif
