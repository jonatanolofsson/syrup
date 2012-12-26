#ifndef SYRUP_COMM_I2C_HPP
#define SYRUP_COMM_I2C_HPP

#include <stdint.h>
#include <syrup/comm/ByteInterface.hpp>
#include <Wire/WireBase.h>
#include <syrup/types.hpp>

namespace syrup {
    class I2CPeripheral : public ByteInterface {
        public:
            typedef U8 address_t;

            WireBase* dev;
            I2CPeripheral(WireBase* dev_, address_t adr)
            : dev(dev_)
            , address(adr)
            {}
            address_t address;
            void beginTransmission() { dev->beginTransmission(address); }
            void endTransmission() { dev->endTransmission(); }
            void send(const U8 a) { dev->send(a); };
            void send(const U8* const a, const int b) { dev->send((U8*)a,b); };
            void send(const int a) { dev->send(a); };
            void send(const int* const a, const int b) { dev->send((int*)a,b); };
            //~ void send(const U8* const a) { dev->send((U8*)a); };
            void command(const U8 a) { beginTransmission(); send(a); endTransmission(); };
            void command(const U8 a, const U8 b) { beginTransmission(); send(a); send(b); endTransmission(); };
            U8 get(U8* const buffer, int N) { return dev->requestFrom(address, N, buffer); }
    };
}
#endif
