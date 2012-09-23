#ifndef SYRUP_COMM_ByteInterface_HPP
#define SYRUP_COMM_ByteInterface_HPP

#include <stdint.h>

namespace communication {
    class ByteInterface {
        public:
            typedef uint8_t address_t;
            ByteInterface(const address_t adr) : address(adr) {}
            address_t address;
            virtual void beginTransmission() {};
            virtual void endTransmission() {};
            virtual void send(uint8_t) = 0;
            virtual void send(uint8_t*, int) = 0;
            virtual void send(int) = 0;
            virtual void send(int*, int) = 0;
            virtual void send(char*) = 0;
            virtual void command(uint8_t) = 0;
            virtual void command(uint8_t, uint8_t) = 0;
            virtual uint8_t get(int, uint8_t*) = 0;
    };
}

#endif
