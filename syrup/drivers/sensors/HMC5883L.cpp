#include <syrup/drivers/sensors/HMC5883L.hpp>
#include <wirish/wirish.h>
#include <syrup/math/math.hpp>
#include <syrup/isr.hpp>

namespace syrup {
    static i2c_msg initMsgs[] = {
        {
            HMC5883L::I2C_ADDRESS,
            2, (U8[]){HMC5883L::CONFIG_A, HMC5883L::AVG8 | HMC5883L::HZ75}
        }
    };
    static __unused i2c_msg sampleData[] = {
        {HMC5883L::I2C_ADDRESS, 6, 0, I2C_MSG_READ},
        {HMC5883L::I2C_ADDRESS, 2, (U8[]){HMC5883L::MODE, HMC5883L::SINGLE}}
    };

    HMC5883L::HMC5883L(i2c_dev* const dev_, const uint8_t exti_pin_)
    :   SuperSensor(),
        device(dev_),
        exti_pin(exti_pin_)
    {
        setup();
        if(exti_pin) {
            pinMode(exti_pin, INPUT);
            attachInterrupt(exti_pin, &classInterruptHandler<HMC5883L>, this, RISING);
            startSample();
        }
    }

    void HMC5883L::setup() {
        i2c_master_xfer(device, initMsgs, 1, 0);
        sampleData[0].data = buffer.raw;
        sampleData[0].arg = this;
        sampleData[0].callback = &i2cMemberCallback<HMC5883L, &HMC5883L::saveData>;

        calibrate();
    }

    void HMC5883L::saveData(struct i2c_msg*) {
        S16 d = (S16)be16toh(buffer.ints[0]);
        if(d == 0) return;
        data[X] += d;
        data[Y] += (S16)be16toh(buffer.ints[1]);
        data[Z] += (S16)be16toh(buffer.ints[2]);
        ++samples;
    }

    void HMC5883L::startSample()
    {
        i2c_master_xfer_async(device, &sampleData[1], 1, 0);
    }

    void HMC5883L::sample()
    {
        i2c_master_xfer_async(device, sampleData, 1 + (exti_pin ? 1 : 0), 0);
    }

    void HMC5883L::calibrate() {
    }
} // namespace syrup
