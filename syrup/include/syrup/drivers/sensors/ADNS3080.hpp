#ifndef SYRUP_DRIVERS_MS5611_HPP
#define SYRUP_DRIVERS_MS5611_HPP

#include <syrup/drivers/sensors/Sensor.hpp>
#include <stdint.h>
#include <libmaple/i2c.h>

namespace syrup {
    class ADNS3080 : public SuperSensor<1> {
        private:
            spi_dev* device;

            uint8_t buffer[7];
            i2c_msg initMsgs[1];
            i2c_msg sampleMsgs[7];
        public:
            enum reg {
                MOTION_BURST    = 0x50
            };
            MS5611(spi_dev* const dev_);
            void setup();
            void sample();
            void saveData(struct spi_msg*);
            void read_PROM();
    };
}
#endif
