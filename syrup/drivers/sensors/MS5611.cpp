#include <syrup/drivers/sensors/MS5611.hpp>
#include <wirish/wirish.h>
#include <syrup/math/math.hpp>

namespace syrup {
    static __unused i2c_msg initMsgs[] = {
        {
            MS5611::I2C_ADDRESS_LOW,
            1, (U8[]){MS5611::RESET}
        }
    };
    static __unused i2c_msg promMsgs[] = {
        {MS5611::I2C_ADDRESS_LOW, 1, (U8[]){MS5611::PROM_C1}},
        {MS5611::I2C_ADDRESS_LOW, 2, 0, I2C_MSG_READ},
        {MS5611::I2C_ADDRESS_LOW, 1, (U8[]){MS5611::PROM_C2}},
        {MS5611::I2C_ADDRESS_LOW, 2, 0, I2C_MSG_READ},
        {MS5611::I2C_ADDRESS_LOW, 1, (U8[]){MS5611::PROM_C3}},
        {MS5611::I2C_ADDRESS_LOW, 2, 0, I2C_MSG_READ},
        {MS5611::I2C_ADDRESS_LOW, 1, (U8[]){MS5611::PROM_C4}},
        {MS5611::I2C_ADDRESS_LOW, 2, 0, I2C_MSG_READ},
        {MS5611::I2C_ADDRESS_LOW, 1, (U8[]){MS5611::PROM_C5}},
        {MS5611::I2C_ADDRESS_LOW, 2, 0, I2C_MSG_READ},
        {MS5611::I2C_ADDRESS_LOW, 1, (U8[]){MS5611::PROM_C6}},
        {MS5611::I2C_ADDRESS_LOW, 2, 0, I2C_MSG_READ},
    };
    static __unused i2c_msg samplePressureMsgs[] = {
        {MS5611::I2C_ADDRESS_LOW, 1, (U8[]){MS5611::READ}},
        {MS5611::I2C_ADDRESS_LOW, 3, 0, I2C_MSG_READ},
        {MS5611::I2C_ADDRESS_LOW, 1, (U8[]){MS5611::CONVERT_D2 | MS5611::OSR512}}
    };
    static __unused i2c_msg sampleTemperatureMsgs[] = {
        {MS5611::I2C_ADDRESS_LOW, 1, (U8[]){MS5611::READ}},
        {MS5611::I2C_ADDRESS_LOW, 3, 0, I2C_MSG_READ},
        {MS5611::I2C_ADDRESS_LOW, 1, (U8[]){MS5611::CONVERT_D1 | MS5611::OSR512}}
    };
    MS5611::MS5611(i2c_dev* const dev_)
    :   Sensor(),
        device(dev_)
    {
        buffer.ints = 0;
        setup();
    }
    void MS5611::setup() {
        for(unsigned i = 0; i < countof(coeffs); ++i) {
            promMsgs[2*i + 1].data = (U8*)&coeffs[i];
            promMsgs[2*i + 1].callback = &i2cMemberCallback<MS5611, &MS5611::saveCoeff>;
        }
        samplePressureMsgs[1].data = &buffer.raw[1];
        samplePressureMsgs[1].arg = this;
        samplePressureMsgs[1].callback = &i2cMemberCallback<MS5611, &MS5611::savePressure>;
        sampleTemperatureMsgs[1].data = &buffer.raw[1];
        sampleTemperatureMsgs[1].arg = this;
        sampleTemperatureMsgs[1].callback = &i2cMemberCallback<MS5611, &MS5611::saveTemperature>;

        i2c_master_xfer(device, initMsgs, 1, 0);
        delay(3);
        read_PROM();
        i2c_master_xfer_async(device, &samplePressureMsgs[2], 1, 0);
        delay(5);
        startTemperatureSample();
    }

    void MS5611::saveTemperature(struct i2c_msg* msg) {
        U32 d = buffer.ints;
        if(0 == d) { return; }
        data[1] = be32toh(d);
    }

    void MS5611::savePressure(struct i2c_msg* msg) {
        U32 d = buffer.ints;
        if(0 == d) { return; }
        data[0] = be32toh(d);
    }

    void MS5611::measure(S32*const dst) {
        *dst = compensatedPressure(data[0], data[1]);
        clear();
    }

    void MS5611::startPressureSample()
    {
        i2c_master_xfer_async(device, samplePressureMsgs, 3, 0);
    }
    void MS5611::startTemperatureSample()
    {
        i2c_master_xfer_async(device, sampleTemperatureMsgs, 3, 0);
    }

    /**
     * Reads factory calibration and store it into object variables.
    */
    void MS5611::read_PROM() {
        i2c_master_xfer(device, promMsgs, 12, 0);
    }
    void MS5611::saveCoeff(struct i2c_msg* msg) {
        U16* coeff = (U16*)msg->data;
        *coeff = be16toh(*coeff);
    }

    S32 MS5611::compensatedPressure(const U32 D1, const U32 D2) {
        S32 dT = D2 - (U32)coeffs[4] * 256;

        S64 OFF = (S64)coeffs[1] * 65536 + (S64)coeffs[3] * dT / 128;
        S64 SENS = (S64)coeffs[0] * 32768 + (S64)coeffs[2] * dT / 256;

        S32 TEMP = 2000 + ((S64) dT * coeffs[5]) / 8388608;

        S32 OFF2 = 0;
        S32 SENS2 = 0;

        if (TEMP < 2000)
        {
            OFF2 = 5 * ((TEMP - 2000) * (TEMP - 2000)) / 2;
            SENS2 = 5 * ((TEMP - 2000) * (TEMP - 2000)) / 4;
        }

        if (TEMP < -1500)
        {
            OFF2 = OFF2 + 7 * ((TEMP + 1500) * (TEMP + 1500));
            SENS2 = SENS2 + 11 * ((TEMP + 1500) * (TEMP + 1500)) / 2;
        }

        OFF = OFF - OFF2;
        SENS = SENS - SENS2;

        S32 P = (D1 * SENS / 2097152 - OFF) / 32768;
        return P;
    }
} // namespace syrup
