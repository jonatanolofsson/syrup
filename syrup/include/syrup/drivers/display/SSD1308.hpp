#ifndef SYRUP_HARDWARE_SSD1308_HPP
#define SYRUP_HARDWARE_SSD1308_HPP

#include <stdint.h>
#include <syrup/drivers/display/BitDisplay.hpp>
#include <syrup/comm/ByteInterface.hpp>
#include <syrup/framebuffer/Framebuffer.hpp>

namespace syrup {
    namespace drivers {
        namespace display {
            using communication::ByteInterface;
            using namespace syrup::graphics;
            class SSD1308 : public BitDisplay {
                private:
                    enum commands {
                        SET_COLUMN_LOWER            = 0x00,
                        SET_COLUMN_UPPER            = 0x10,
                        NORMAL_COLUMNS              = 0xA0,
                        REVERSE_COLUMNS             = 0xA1,
                        SET_MEMORY_ADDRESSING_MODE  = 0x20,
                        SET_COLUMN_ADDRESS          = 0x21,
                        SET_PAGE_ADDRESS            = 0x22,
                        SET_PAGE_START_ADDRESS      = 0xB0,
                        SET_CONTRAST                = 0x81,
                        DATA_MODE                   = 0x40,
                        COMMAND_MODE                = 0x80,
                        SET_NORMAL_DISPLAY          = 0xA6,
                        SET_INVERSE_DISPLAY         = 0xA7,
                        SET_DISPLAY_POWER_OFF       = 0xAE,
                        SET_DISPLAY_POWER_ON        = 0xAF
                    };
                    enum addressing_modes {
                        HORIZONTAL_ADDRESSING_MODE  = 0x00,
                        VERTICAL_ADDRESSING_MODE    = 0x01,
                        PAGE_ADDRESSING_MODE        = 0x02
                    };

                public:
                    enum i2c_address {
                        I2C_ADDRESS                 = 0x3C
                    };

                    ByteInterface* port;
                    SSD1308(ByteInterface* port_);

                    uint8_t width() { return 128; }
                    uint8_t height() { return 64; }
                    void init();
                    void flush(Framebuffer* fb);

                private:
                    void setPage(uint8_t row);
                    void setCol(uint8_t col);
                    void setPages(uint8_t rowmin, uint8_t rowmax);
                    void setCols(uint8_t colmin, uint8_t colmax);
            };
        }
    }
}
#endif
