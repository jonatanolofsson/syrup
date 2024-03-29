#pragma once

#include <stdint.h>
#include <string.h>
#include <syrup/utils/utils.hpp>
#include <wirish/wirish.h>
#include <string.h>
#include <syrup/isr.hpp>
#include <syrup/types.hpp>

namespace syrup {
    template<int N, typename DATA, typename MEAS>
    class Sensor
    {
        private:
            typedef Sensor<N, DATA, MEAS> Self;
        public:
            typedef DATA    data_t;
            typedef MEAS    measurement_t;
            typedef data_t  data_a[N];
            data_t data[N];

            virtual void sample() = 0;
            virtual void isr() {
                isr::queue(&isr::invokeMember<Self, &Self::lowerIsr>, this);
            }
            void lowerIsr() {
                sample();
            }
            Sensor() {
                Sensor::clear();
            }
            virtual void clear() {
                memset(data, 0, sizeof(data));
            }

            virtual void measure(measurement_t* buffer) {
                memcpy(buffer, data, sizeof(data));
                clear();
            }
    };

    template<int N, typename DATA = S32, typename MEAS = U16, typename SAMPL = U16>
    class SuperSensor : public Sensor<N, DATA, MEAS>
    {
        public:
            typedef Sensor<N,DATA,MEAS> Parent;
            using Parent::data;

            typedef DATA measurement_t;
            typedef SAMPL samplecount_t;
            samplecount_t samples;
            SuperSensor() : Parent(), samples(0) {}
            void clear() {
                samples = 0;
                Parent::clear();
            }
            void measure(measurement_t*const buffer, samplecount_t*const nofMeasurements = nullptr) {
                if(nofMeasurements != nullptr) *nofMeasurements = samples;
                memcpy(buffer, data, sizeof(data));
                clear();
            }
    };
} // namespace syrup
