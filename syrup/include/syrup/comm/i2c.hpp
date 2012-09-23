#ifndef SYRUP_COMM_I2C_HPP
#define SYRUP_COMM_I2C_HPP

#include <stdint.h>
#include <syrup/comm/ByteInterface.hpp>
#include <Wire/WireBase.h>

namespace communication {
    class I2CPeripheral : public ByteInterface {
        public:
            WireBase* dev;
            I2CPeripheral(WireBase* dev_, address_t adr) : ByteInterface(adr), dev(dev_) {}
            void beginTransmission() { dev->beginTransmission(address); }
            void endTransmission() { dev->endTransmission(); }
            void send(uint8_t a) { dev->send(a); };
            void send(uint8_t* a, int b) { dev->send(a,b); };
            void send(int a) { dev->send(a); };
            void send(int* a, int b) { dev->send(a,b); };
            void send(char* a) { dev->send(a); };
            void command(uint8_t a) { beginTransmission(); send(a); endTransmission(); };
            void command(uint8_t a, uint8_t b) { beginTransmission(); send(a); send(b); endTransmission(); };
            uint8_t get(int N, uint8_t* buffer) { return dev->requestFrom(address, N, buffer); }
    };
}

#endif
