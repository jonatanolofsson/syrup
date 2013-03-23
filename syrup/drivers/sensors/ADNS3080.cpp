#include <syrup/drivers/sensors/MS5611.hpp>
#include <wirish/wirish.h>
#include <syrup/math/math.hpp>

namespace syrup {
    MS5611::MS5611(i2c_dev* const dev_, const U8 adr_)
    :   SuperSensor(),
        device(dev_),
        address(adr_)
    {
        setup();
    }

    void MS5611::setup() {

        initMsgs[0].addr = address;
        initMsgs[0].length = 1;
        initMsgs[0].data = (U8[]){RESET};
        i2c_master_xfer_async(device, initMsgs, 1, 0);
        //~ read_PROM();

        sampleMsgs[0].addr = address;
        sampleMsgs[0].length = 1;
        sampleMsgs[0].data = (U8[]){READ};

        sampleMsgs[1].addr = address;
        sampleMsgs[1].flags = I2C_MSG_READ;
        sampleMsgs[1].length = 3;
        sampleMsgs[1].data = buffer;
        sampleMsgs[1].arg = this;
        sampleMsgs[1].callback = &i2cMemberCallback<MS5611, &MS5611::saveData>;

        sampleMsgs[2].addr = address;
        sampleMsgs[2].length = 1;
        sampleMsgs[2].data = (U8[]){CONVERT_D1 | OSR4096};
    }


    void MS5611::saveData(struct i2c_msg*) {
        uint8_t d = convert_endian(buffer[4],buffer[5]);
        if(0 == d) { return; }
        data[bufferswitch][0] += d;
        ++samples[bufferswitch][0];
    }

    void MS5611::sample()
    {
        spi_master_xfer_async(device, sampleMsgs, 3, 0);
    }
} // namespace syrup
