#include <syrup/drivers/sensors/MS5611.hpp>
#include <wirish/wirish.h>
#include <syrup/math/math.hpp>

namespace syrup {
    static i2c_msg initMsgs[] = {
        {
            MS5611::I2C_ADDRESS_LOW,
            1, (U8[]){MS5611::RESET}
        }
    };
    static i2c_msg sampleMsgs[] = {
        {
            MS5611::I2C_ADDRESS_LOW,
            1, (U8[]){MS5611::READ}
        },
        {
            MS5611::I2C_ADDRESS_LOW,
            3, 0,
            I2C_MSG_READ
        },
        {
            MS5611::I2C_ADDRESS_LOW,
            1, (U8[]){MS5611::CONVERT_D1 | MS5611::OSR4096}
        }
    };
    MS5611::MS5611(i2c_dev* const dev_)
    :   SuperSensor(),
        device(dev_)
    {
        setup();
    }

    void MS5611::setup() {
        i2c_master_xfer(device, initMsgs, 1, 0);
        //~ read_PROM();

        sampleMsgs[1].data = buffer;
        sampleMsgs[1].arg = this;
        sampleMsgs[1].callback = &i2cMemberCallback<MS5611, &MS5611::saveData>;
    }


    void MS5611::saveData(struct i2c_msg*) {
        uint8_t d = (buffer[0] << 16) | (buffer[1] << 8) | (buffer[2]);
        if(0 == d) { return; }
        data[bufferswitch][0] += d;
        ++samples[bufferswitch];
    }

    void MS5611::sample()
    {
        i2c_master_xfer_async(device, sampleMsgs, 3, 0);
    }


    /**
     * Reads factory calibration and store it into object variables.
    */
    void MS5611::read_PROM() {
    }
} // namespace syrup
