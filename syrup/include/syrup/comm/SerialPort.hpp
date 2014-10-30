#ifndef SYRUP_COMM_SERIAL_HPP
#define SYRUP_COMM_SERIAL_HPP

#include <stdint.h>
#include <syrup/comm/ByteInterface.hpp>
#include <wirish/Print.h>
#include <syrup/types.hpp>

#define USE_DMA 1

namespace syrup {
    template<typename T, int BUFFERSIZE = 100>
    class SerialPort : public ByteInterface {
        #if USE_DMA
        private:
            typedef ByteInterface Parent;
            volatile U8 buffer[2][BUFFERSIZE];
            dma_message txMsgs[2];
            dma_message rxMsg;
            int receivedBytes;
            bool reading;
        public:
            dma_message::dma_msg_callback rxCallback;
            dma_message::dma_msg_callback txCallback;
        #endif
        public:
            T* dev;
            SerialPort(T* const dev_) : Parent(),
            #if USE_DMA
            receivedBytes(0), reading(false),
            rxCallback(NULL), txCallback(NULL),
            #endif
            dev(dev_) {}
            void send(const U8 a) { dev->write(a); };
            void send(const U8* const a, const int b) {
                #if USE_DMA
                static bool n = 0;
                ASSERT(b <= BUFFERSIZE);
                memcpy((void*)buffer[n], a, b);
                dev->write(txMsgs[n](buffer[n],b,txCallback));
                n = !n;
                txMsgs[n].wait();
                #else
                dev->write(a,b);
                #endif
            };
            void send(const int a) { dev->print(a); };
            void send(const int* const a, const int b) { dev->write(a,sizeof(int)*b); };
            void send(const U8* const a) { dev->write(a); };
            void command(const U8 a) { send(a); };
            void command(const U8 a, const U8 b) { send(a); send(b);};
            U8 get(U8* buffer, const int N) { return dev->read(rxMsg(buffer, N, rxCallback)); }
    };
}
#endif
