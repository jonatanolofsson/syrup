#include <syrup/drivers/sensors/HMC5883L.hpp>
#include <wirish/wirish.h>
#include <syrup/math/math.hpp>
#include <syrup/utils/utils.hpp>

namespace syrup {
    static i2c_msg initMsgs[] = {
        {
            HMC5883L::I2C_ADDRESS,
            2, (U8[]){HMC5883L::CONFIG_A, HMC5883L::AVG8 | HMC5883L::HZ75}
        }
    };
    static i2c_msg sampleMsgs[] = {
        {
            HMC5883L::I2C_ADDRESS,
            1, (U8[]){HMC5883L::MODE, HMC5883L::SINGLE}
        },
        {
            HMC5883L::I2C_ADDRESS,
            1, (U8[]){HMC5883L::READ}
        },
        {
            HMC5883L::I2C_ADDRESS,
            6, 0,
            I2C_MSG_READ
        }
    };
    HMC5883L::HMC5883L(i2c_dev* const dev_, const uint8_t exti_pin_)
    :   SuperSensor(),
        device(dev_),
        exti_pin(exti_pin_)
    {
        setup();
        if(exti_pin) {
            pinMode(exti_pin, INPUT_PULLUP);
            attachInterrupt(exti_pin, classInterruptHandler<HMC5883L>, this, RISING);
        }
    }

    void HMC5883L::setup() {
        i2c_master_xfer(device, initMsgs, 1, 0);
        sampleMsgs[2].data = buffer;
        sampleMsgs[2].arg = this;
        sampleMsgs[2].callback = &i2cMemberCallback<HMC5883L, &HMC5883L::saveData>;

        calibrate();
    }

    void HMC5883L::saveData(struct i2c_msg*) {
        uint16_t d = be16toh(*(U16*)&buffer[0]);
        if(d == 0) return;
        data[bufferswitch][X] += d;
        ++samples[bufferswitch][X];
        data[bufferswitch][Y] += be16toh(*(U16*)&buffer[2]);
        ++samples[bufferswitch][Y];
        data[bufferswitch][Z] += be16toh(*(U16*)&buffer[4]);
        ++samples[bufferswitch][Z];
    }

    void HMC5883L::sample()
    {
        i2c_master_xfer_async(device, sampleMsgs, 3, 0);
    }

    void HMC5883L::calibrate() {
    }
} // namespace syrup
