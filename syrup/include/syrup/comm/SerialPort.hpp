#ifndef SYRUP_COMM_SERIAL_HPP
#define SYRUP_COMM_SERIAL_HPP

#include <stdint.h>
#include <syrup/comm/ByteInterface.hpp>
#include <wirish/Print.h>
#include <syrup/types.hpp>

namespace syrup {
    template<typename T>
    class SerialPort : public ByteInterface {
        public:
            T* dev;
            SerialPort(T* const dev_) : dev(dev_) {}
            void send(const U8 a) { dev->write(a); };
            void send(const U8* const a, const int b) { dev->write(a,b); };
            void send(const int a) { dev->print(a); };
            void send(const int* const a, const int b) { dev->write(a,sizeof(int)*b); };
            void send(const U8* const a) { dev->write(a); };
            void command(const U8 a) { send(a); };
            void command(const U8 a, const U8 b) { send(a); send(b);};
            U8 get(U8* buffer, const int N) { dev->read(buffer, N); return N; }
    };
}
#endif
