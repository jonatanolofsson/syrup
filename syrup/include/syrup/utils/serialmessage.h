#ifndef SERIAL_MESSAGE_HPP_
#define SERIAL_MESSAGE_HPP_

#define SERIAL_MESSAGE_SEPARATOR        0xAA

struct MessageHeader {
    uint16_t    id;
    uint16_t    length;
    uint8_t     headerCRC;
    uint8_t     bodyCRC;
};
struct Message {
    char*       body;
    MessageHeader header;
    uint8_t     headerCRC;
    uint8_t     bodyCRC;
};

#endif
