#ifndef SYRUP_DRIVERS_ADNS3080_HPP_
#define SYRUP_DRIVERS_ADNS3080_HPP_

#include <syrup/drivers/sensors/Sensor.hpp>
#include <stdint.h>
#include <wirish/wirish.h>

namespace syrup {
    class ADNS3080 : public SuperSensor<2> {
        private:
            HardwareSPI* device;
            U8 sspin;

            U8 buffer[7];
            dma_message trigger;
            dma_message message;
        public:
            enum reg {
                MOTION_BURST    = 0x50
            };
            ADNS3080(HardwareSPI* const dev_);
            void printBuffer();
            void setup();
            void sample();
            void getData(dma_message*, dma_irq_cause);
            void saveData(dma_message*, dma_irq_cause);
    };
}
#endif
