#ifndef SYRUP_SERIALMULTIPLEXER_HPP_
#define SYRUP_SERIALMULTIPLEXER_HPP_

#include <libmaple/usart.h>
#include <syrup/utils/serialmessage.h>
#include <syrup/utils/utils.hpp>

namespace syrup {
    typedef Message SerialMessage;
    typedef void(*SerialReceiver)(SerialMessage*, usart_dev*);
    typedef MessageHeader SerialMessageHeader;
    template<int BUFFER_SIZE, int MESSAGE_LIMIT>
    class SerialMultiplexer {
        private:
            union {
                SerialMessage message;
                char buffer[sizeof(MessageHeader) + BUFFER_SIZE + sizeof(char*)];
            } message;

            usart_dev* device;
            SerialReceiver* callbacks;
            struct readerConfig {
                typedef void(SerialMultiplexer<BUFFER_SIZE, MESSAGE_LIMIT>::*handler_t)(char*,int);
                handler_t handler;
                int size;
                int offset;
                uint8_t* crc;
            };
            readerConfig reader;

            void read() {
                while(reader.size > 0) {
                    while (usart_data_available(device) && reader.size > 0) {
                        *(reader.crc) ^= (message.buffer[reader.offset] = usart_getc(device));
                        --reader.size;
                        ++reader.offset;
                    }
                    if(reader.size > 1) taskYIELD();
                }
            }

            void restartReader() {
                reader = (readerConfig) {
                    &SerialMultiplexer<BUFFER_SIZE, MESSAGE_LIMIT>::readSeparator,
                    1,
                    sizeof(char*),
                    &message.message.headerCRC
                };
                message.message.headerCRC = 0;
            }
            void readBody(char* r, int) {
                if(bodyIsConsistent()) {
                    if(message.message.header.id < MESSAGE_LIMIT && callbacks[message.message.header.id]) {
                        callbacks[message.message.header.id](&message.message, device);
                    }
                    restartReader();
                }
            }
            void readHeader(char* r, int size) {
                if(headerIsConsistent()) {
                    reader.handler  = &SerialMultiplexer<BUFFER_SIZE, MESSAGE_LIMIT>::readBody;
                    reader.size     = message.message.header.length;
                    reader.offset   += sizeof(message.message.headerCRC) + sizeof(message.message.bodyCRC); // Don't overwrite CRC checksums in message struct (not header)
                    reader.crc      = &message.message.bodyCRC;
                    message.message.bodyCRC = 0;
                } else restartReader();
            }
            void readSeparator(char* r, int size) {
                if(*r == SERIAL_MESSAGE_SEPARATOR) {
                    reader.handler  = &SerialMultiplexer<BUFFER_SIZE, MESSAGE_LIMIT>::readHeader;
                    reader.size     = sizeof(MessageHeader);
                    reader.offset   += 1;
                    message.message.headerCRC = 0;
                } else restartReader();
            }

            bool bodyIsConsistent() {
                return (message.message.header.bodyCRC == message.message.bodyCRC);
            }
            bool headerIsConsistent() {
                return (message.message.header.headerCRC == message.message.headerCRC) && message.message.header.length <= BUFFER_SIZE;
            }

        public:
            SerialMultiplexer(usart_dev* dev_, SerialReceiver* callbacks_)
            :   device(dev_),
                callbacks(callbacks_)
            {
                Serial1.println("Serial multiplexer init");
                message.message.body = ((char*)&message.message) + sizeof(Message);
                xTaskCreate(classThreadInitializer<SerialMultiplexer>,
                            (signed portCHAR *)"SerialMultiplexer",
                            configMINIMAL_STACK_SIZE,
                            this,
                            tskIDLE_PRIORITY + 2,
                            NULL);
                Serial1.println("Serial multiplexer init: dispatched tasks");
            }
            void run() {
                Serial1.println("Serial multiplexer task: started");
                char* bfr;
                int size;

                restartReader();
                for(;;) {
                    bfr = &message.buffer[reader.offset];
                    size = reader.size;
                    (this->*(reader.handler))(bfr, size);
                    read();
                }
            }
    };
}
#endif
