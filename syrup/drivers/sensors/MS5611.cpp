#include <syrup/drivers/sensors/MS5611.hpp>
#include <wirish/wirish.h>
#include <syrup/math/math.hpp>

namespace syrup {
    namespace drivers {
        namespace sensors {
            using communication::ByteInterface;
            MS5611 *ms5611low = 0;
            MS5611 *ms5611high = 0;

            void __MS5611_timer_interrupt_handler(void) {
                if(ms5611low)   ms5611low->timer_sample();
                if(ms5611high)  ms5611high->timer_sample();
            }

            MS5611::MS5611(ByteInterface* port_, bool use_timer)
            :   SuperSensor(1),
                port(port_)
            {
                setup();
                if(use_timer) {
                    if(port->address == I2C_ADDRESS_LOW)        ms5611low = this;
                    else if(port->address == I2C_ADDRESS_HIGH)  ms5611high = this;

                    port->command(CONVERT_D1 | OSR4096);
                    Timer4.setPeriod(10000);
                    Timer4.attachInterrupt(1, &__MS5611_timer_interrupt_handler);
                    Timer4.refresh();
                }
            }

            void MS5611::setup() {
                port->command(RESET);
                delay(1000);
                //~ read_PROM();
            }

            void MS5611::sample()
            {
                port->command(CONVERT_D1 | OSR4096);
                delay(9);
                port->command(READ);
                uint8_t buffer[3];
                port->get(3, buffer);
                data[0] += convert_endian(buffer[0], buffer[1], buffer[2]);
                ++samples[0];
            }

            void MS5611::timer_sample()
            {
                port->command(READ);
                uint8_t buffer[3];
                port->get(3, buffer);
                uint8_t d = convert_endian(buffer[0], buffer[1], buffer[2]);
                if(d == 0) return;
                data[0] += d;
                ++samples[0];
                port->command(CONVERT_D1 | OSR4096);
            }


            /**
             * Reads factory calibration and store it into object variables.
            */
            void MS5611::read_PROM() {
                uint8_t buffer[2];
                for (int i = 1; i < PROM_REG_COUNT; ++i) {
                    port->command(PROM_BASE_ADDR + (i * 2));
                    port->get(2, buffer);
                    coeff[i] = convert_endian(buffer[0], buffer[1]);
                }
            }
        } // namespace sensors
    } // namespace drivers
} // namespace syrup
