#ifndef SYRUP_DRIVERS_SENSOR_HPP
#define SYRUP_DRIVERS_SENSOR_HPP

#include <stdint.h>
#include <syrup/utils/utils.hpp>
#include <wirish/wirish.h>
#include <string.h>

namespace syrup {
    namespace drivers {
        namespace sensors {
            class Sensor
            {
                protected:
                    unsigned int N;
                public:
                    typedef int32_t         data_t;
                    typedef uint16_t    raw_t;
                    typedef uint16_t    measurement_t;
                    bool bufferswitch;
                    data_t* data[2];

                    struct recordingSettings {
                        measurement_t* buffer;
                        uint32_t buffersize;
                        uint32_t filled;
                        uint32_t modulo;
                        uint32_t counter;
                        bool active;
                    } recording;

                    virtual void sample() = 0;
                    virtual void run() {sample();}
                    Sensor(const unsigned int N_) : N(N_), bufferswitch(false) {
                        data[0] = new data_t[N];
                        data[1] = new data_t[N];
                        Sensor::clear();
                    }
                    virtual void clear() {
                        memset(data[0], 0, N*sizeof(data_t));
                        memset(data[1], 0, N*sizeof(data_t));
                    }
                    virtual ~Sensor() {
                        delete[] data;
                    }
                    virtual void switch_buffer() {
                        bufferswitch = !bufferswitch;
                    }
                    virtual void make_record(measurement_t* buffer) {
                        if(recording.active) {
                            if((++recording.counter % recording.modulo) == 0) {
                                if(recording.buffersize - recording.filled >= N) {
                                    memcpy(&recording.buffer[recording.filled], buffer, N*sizeof(measurement_t));
                                    recording.filled += N;
                                } else {
                                    recording.active = false;
                                }
                            }
                        }
                    }
                    virtual void measure(measurement_t* buffer) {
                        bool mbuffer = bufferswitch;
                        switch_buffer();
                        for(unsigned int i = 0; i < N; ++i) {
                            buffer[i] = data[mbuffer][i];
                        }
                        make_record(buffer);
                        memset(data[!bufferswitch], 0, sizeof(data[0]));
                    }

                    virtual void average(measurement_t* buffer, int samples, int delayTime) {
                        data_t* buf = new data_t[N];
                        memset(buf, 0, N*sizeof(data_t));
                        clear();
                        for(int i = 0;i < samples; ++i) {
                            DELAY(delayTime);
                            measure(buffer);
                            for(unsigned int j = 0; j < N; ++j) {
                                buf[j] += buffer[j];
                            }
                        }
                        for(unsigned int j = 0; j < N; ++j) {
                            buffer[j] = buf[j] / samples;
                        }
                        delete[] buf;
                    }

                    void virtual record(measurement_t* buf, unsigned int buffersize, unsigned int mod = 1) {
                        recording.buffer = buf;
                        recording.buffersize = buffersize;
                        recording.filled = 0;
                        recording.modulo = mod;
                        recording.counter = 0;
                        recording.active = true;
                    }
            };

            class SuperSensor : public Sensor
            {
                public:
                    typedef uint16_t   samplecount_t;
                    samplecount_t*   samples[2];
                    SuperSensor(int N) : Sensor(N) {
                        samples[0] = new samplecount_t[N];
                        samples[1] = new samplecount_t[N];
                        clear();
                    }
                    virtual ~SuperSensor() {
                        delete[] samples;
                    }
                    void clear() {
                        Sensor::clear();
                        memset(samples[0], 0, sizeof(samples[0]));
                        memset(samples[1], 0, sizeof(samples[1]));
                    }
                    void measure(measurement_t* buffer) {
                        bool mbuffer = bufferswitch;
                        switch_buffer();

                        for(unsigned int i = 0; i < N; ++i) {
                            buffer[i] = data[mbuffer][i] / samples[mbuffer][i];
                        }
                        make_record(buffer);
                        memset(data[mbuffer], 0, sizeof(data[0]));
                        memset(samples[mbuffer], 0, sizeof(samples[0]));
                    }
            };
        } // namespace sensors
    } // namespace drivers
} // namespace syrup

#endif
