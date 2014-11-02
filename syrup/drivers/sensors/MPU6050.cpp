#include <syrup/drivers/sensors/MPU6050.hpp>
#include <wirish/wirish.h>
#include <syrup/isr.hpp>
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
        {MPU6050::I2C_ADDRESS_LOW, 1, (U8[]){MPU6050::REG_SENSOR_DATA}},
        {MPU6050::I2C_ADDRESS_LOW, 14, 0, I2C_MSG_READ}
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
            attachInterrupt(exti_pin, &classInterruptHandler<MPU6050>, this, RISING);
        }
    }

    void MPU6050::setup() {
        i2c_master_xfer_async(device, initMsgs, 4, 0);
        sampleMsgs[1].data = buffer.raw;
        sampleMsgs[1].arg = this;
        sampleMsgs[1].callback = &i2cMemberCallback<MPU6050, &MPU6050::saveData>;
    }

    void MPU6050::saveData(struct i2c_msg*) {
        data[ACC_X]  += (S16)be16toh(buffer.ints[0]);
        data[ACC_Y]  += (S16)be16toh(buffer.ints[1]);
        data[ACC_Z]  += (S16)be16toh(buffer.ints[2]);
        data[GYRO_X] += (S16)be16toh(buffer.ints[4]);
        data[GYRO_Y] += (S16)be16toh(buffer.ints[5]);
        data[GYRO_Z] += (S16)be16toh(buffer.ints[6]);
        ++samples;
    }

    void MPU6050::sample()
    {
        i2c_master_xfer_async(device, sampleMsgs, 2, 0);
    }
} // namespace syrup
