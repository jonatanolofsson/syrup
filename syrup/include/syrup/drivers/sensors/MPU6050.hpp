#ifndef SYRUP_DRIVERS_MPU6050_HPP
#define SYRUP_DRIVERS_MPU6050_HPP

#include <syrup/drivers/sensors/Sensor.hpp>
#include <stdint.h>
#include <syrup/comm/ByteInterface.hpp>

namespace syrup {
    class MPU6050 : public SuperSensor<6> {
        private:
            uint16_t previous_accX;
            uint8_t exti_pin;
        public:
            enum i2c_address {
                I2C_ADDRESS_LOW     = 0x68,
                I2C_ADDRESS_HIGH    = 0x69
            };
            enum registers {
                CONFIG          = 0x1A,
                GYRO_CONFIG     = 0x1B,
                ACC_CONFIG      = 0x1C,
                I2C_MST_CTRL    = 0x24,
                INT_PIN_CFG     = 0x37,
                INT_ENABLE      = 0x38,
                REG_SENSOR_DATA = 0x3B,
                USER_CTRL       = 0x6A,
                PWR_MGMT_1      = 0x6B,
            };
            enum sensors {
                ACC_X,
                ACC_Y,
                ACC_Z,
                GYRO_X,
                GYRO_Y,
                GYRO_Z
            };
            enum commands {
                CLEAR           = 0x00,
                RESET           = 0x80,
                DATA_RDY_EN     = 0x01,
                CLOCK_PLL_XGYRO = 0x01,
                TEMP_DIS        = 0x04,

                I2C_BYPASS_EN   = 0x02,

            };
            enum gyro_range {
                MAX_250     = 0x00,
                MAX_500     = 0x01,
                MAX_1000    = 0x02,
                MAX_2000    = 0x03
            };
            enum accel_range {
                MAX_2G      = 0x00,
                MAX_4G      = 0x01,
                MAX_8G      = 0x02,
                MAX_16G     = 0x03
            };
            ByteInterface* port;
            MPU6050(ByteInterface* port_, const uint8_t exti_pin_ = 0);
            void setup();
            void sample();
    };
}
#endif
