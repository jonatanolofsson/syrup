#include <syrup/drivers/sensors/MPU6050.hpp>
#include <wirish/wirish.h>
#include <syrup/math/math.hpp>
#include <syrup/utils/utils.hpp>

namespace syrup {
    MPU6050::MPU6050(ByteInterface* port_, const uint8_t exti_pin_)
    :   SuperSensor(),
        previous_accX(0),
        exti_pin(exti_pin_),
        port(port_)
    {
        setup();
        if(exti_pin) {
            pinMode(exti_pin, INPUT_PULLDOWN);
            attachInterrupt(exti_pin, classInterruptHandler<MPU6050>, this, RISING);
        }
    }

    void MPU6050::setup() {
        //~ port->command(PWR_MGMT_1, RESET);
        //~ port->command(PWR_MGMT_1, 0);
        port->command(PWR_MGMT_1, CLOCK_PLL_XGYRO);
        port->command(CONFIG, 2);
        port->command(INT_ENABLE, DATA_RDY_EN);
        port->command(INT_PIN_CFG, I2C_BYPASS_EN);
    }

    void MPU6050::sample()
    {
        port->command(REG_SENSOR_DATA);
        uint8_t buffer[14];
        port->get(buffer, 14);
        uint16_t d;
        d = convert_endian(buffer[0], buffer[1]);
        if(d == 0) return;
        if(d != previous_accX) {
            data[bufferswitch][ACC_X] += d;
            ++samples[bufferswitch][ACC_X];
            data[bufferswitch][ACC_Y] += convert_endian(buffer[2], buffer[3]);
            ++samples[bufferswitch][ACC_Y];
            data[bufferswitch][ACC_Z] += convert_endian(buffer[4], buffer[5]);
            ++samples[bufferswitch][ACC_Z];
        }
        data[bufferswitch][GYRO_X] += convert_endian(buffer[8], buffer[9]);
        ++samples[bufferswitch][GYRO_X];
        data[bufferswitch][GYRO_Y] += convert_endian(buffer[10], buffer[11]);
        ++samples[bufferswitch][GYRO_Y];
        data[bufferswitch][GYRO_Z] += convert_endian(buffer[12], buffer[13]);
        ++samples[bufferswitch][GYRO_Z];
    }
} // namespace syrup
