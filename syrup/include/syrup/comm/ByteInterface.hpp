#ifndef SYRUP_COMM_ByteInterface_HPP
#define SYRUP_COMM_ByteInterface_HPP

#include <syrup/types.hpp>

namespace syrup {
    class ByteInterface {
        public:
            virtual void open() {};
            virtual void close() {};
            virtual void beginTransmission() {};
            virtual void endTransmission() {};
            virtual void send(const U8) = 0;
            virtual void send(const U8* const, int) = 0;
            virtual void send(const int) = 0;
            //~ virtual void send(int*, int) = 0;
            //~ virtual void send(const U8* const) = 0;
            virtual void command(const U8) = 0;
            virtual void command(const U8, const U8) = 0;
            virtual U8 get(U8*, const int) = 0;
    };

    int open(ByteInterface* const iface);
    inline void close(const int) {}
    int write(const int descriptor, const U8* const data, const int length);
    int read(const int descriptor, U8* const buf, const int length);
}

#endif
