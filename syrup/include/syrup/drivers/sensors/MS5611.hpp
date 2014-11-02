#ifndef SYRUP_DRIVERS_MS5611_HPP
#define SYRUP_DRIVERS_MS5611_HPP

#include <syrup/drivers/sensors/Sensor.hpp>
#include <syrup/types.hpp>
#include <libmaple/i2c.h>

namespace syrup {
    class MS5611 : public Sensor<2, U32, U32> {
        private:
            i2c_dev* device;

            union {
                U8 raw[4];
                U32 ints;
            } buffer;

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
                PROM_C1         = PROM_BASE_ADDR + 0x2,
                PROM_C2         = PROM_BASE_ADDR + 0x4,
                PROM_C3         = PROM_BASE_ADDR + 0x6,
                PROM_C4         = PROM_BASE_ADDR + 0x8,
                PROM_C5         = PROM_BASE_ADDR + 0xA,
                PROM_C6         = PROM_BASE_ADDR + 0xC,
            };
            uint16_t coeffs[6];
            MS5611(i2c_dev* const dev_);
            void setup();
            void sample(){};
            void measure(S32*const);
            void startTemperatureSample();
            void startPressureSample();
            void savePressure(struct i2c_msg*);
            void saveTemperature(struct i2c_msg*);
            void saveCoeff(struct i2c_msg*);
            void read_PROM();
            S32 compensatedPressure(const U32, const U32);
    };
}
#endif
