#include <syrup/drivers/sensors/MPU6050.hpp>
#include <wirish/wirish.h>
#include <syrup/utils/utils.hpp>
#include <syrup/math/math.hpp>

void print(int v, int base = 10);

namespace syrup {
    static i2c_msg initMsgs[] = {
        {
            MPU6050::I2C_ADDRESS_LOW,
            2, (U8[]){MPU6050::PWR_MGMT_1, MPU6050::CLOCK_PLL_XGYRO}
        },
        {
            MPU6050::I2C_ADDRESS_LOW,
            2, (U8[]){MPU6050::CONFIG, 2}
        },
        {
            MPU6050::I2C_ADDRESS_LOW,
            2, (U8[]){MPU6050::INT_ENABLE, MPU6050::DATA_RDY_EN}
        },
        {
            MPU6050::I2C_ADDRESS_LOW,
            2, (U8[]){MPU6050::INT_PIN_CFG, MPU6050::I2C_BYPASS_EN}
        }
    };

    static i2c_msg sampleMsgs[] = {
        {
            MPU6050::I2C_ADDRESS_LOW,
            1, (U8[]){MPU6050::REG_SENSOR_DATA}
        },
        {
            MPU6050::I2C_ADDRESS_LOW,
            14, 0,
            I2C_MSG_READ
        }
    };

    MPU6050::MPU6050(i2c_dev* dev_, const uint8_t exti_pin_)
    :   SuperSensor(),
        device(dev_),
        previous_accX(0),
        exti_pin(exti_pin_)
    {
        setup();
        if(exti_pin) {
            pinMode(exti_pin, INPUT_PULLDOWN);
            attachInterrupt(exti_pin, classInterruptHandler<MPU6050>, this, RISING);
        }
    }

    void MPU6050::setup() {
        i2c_master_xfer_async(device, initMsgs, 4, 0);
        sampleMsgs[1].data = buffer;
        sampleMsgs[1].arg = this;
        sampleMsgs[1].callback = &i2cMemberCallback<MPU6050, &MPU6050::saveData>;
    }

    void MPU6050::saveData(struct i2c_msg*) {
        data[bufferswitch][ACC_X] += be16toh(*(uint16_t*)&buffer[0]);   ++samples[bufferswitch][ACC_X];
        data[bufferswitch][ACC_Y] += be16toh(*(uint16_t*)&buffer[2]);   ++samples[bufferswitch][ACC_Y];
        data[bufferswitch][ACC_Z] += be16toh(*(uint16_t*)&buffer[4]);   ++samples[bufferswitch][ACC_Z];
        data[bufferswitch][GYRO_X] += be16toh(*(uint16_t*)&buffer[8]);  ++samples[bufferswitch][GYRO_X];
        data[bufferswitch][GYRO_Y] += be16toh(*(uint16_t*)&buffer[10]); ++samples[bufferswitch][GYRO_Y];
        data[bufferswitch][GYRO_Z] += be16toh(*(uint16_t*)&buffer[12]); ++samples[bufferswitch][GYRO_Z];
    }

    void MPU6050::sample()
    {
        i2c_master_xfer_async(device, sampleMsgs, 2, 0);
    }
} // namespace syrup
