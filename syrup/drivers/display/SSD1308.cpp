#include <syrup/drivers/display/SSD1308.hpp>
#include <wirish/wirish_time.h>
#include <wirish/io.h>
namespace syrup {
    namespace drivers {
        namespace display {
            using namespace syrup::graphics;

            SSD1308::SSD1308(ByteInterface* port_)
                : port(port_)
            {
                init();
            }

            void SSD1308::init()
            {
                port->command(COMMAND_MODE, SET_DISPLAY_POWER_OFF);
                delay(50);
                port->command(COMMAND_MODE, SET_DISPLAY_POWER_ON);
                delay(50);
            }
            void SSD1308::flush(Framebuffer* fb)
            {
                uint8_t w = fb->limits.x2-fb->limits.x1 + 1;
                uint8_t page = fb->limits.y1 / 8;
                uint8_t last_page = fb->limits.y2 / 8;

                //~ SerialUSB.print("Flush: x1: "); SerialUSB.print(fb->limits.x1);
                //~ SerialUSB.print("w: "); SerialUSB.print(w);

                for(; page <= last_page; ++page)
                {
                    setPage(page);
                    setCol(fb->limits.x1);
                    port->beginTransmission();
                        port->send(DATA_MODE);
                        port->send(&fb->data[fb->limits.x1 + fb->width * page], w);
                    port->endTransmission();
                }
            }
            void SSD1308::setPage(uint8_t row)
            {
                //~ SerialUSB.print("Set page: "); SerialUSB.println(row);
                port->command(COMMAND_MODE, 0xb0+row); //set page address
            }
            void SSD1308::setCol(uint8_t col)
            {
                //~ SerialUSB.print("Set col: "); SerialUSB.println(col);
                port->command(COMMAND_MODE, SET_COLUMN_LOWER | (col&0x0f));
                port->command(COMMAND_MODE, SET_COLUMN_UPPER | ((col>>4)&0x0f));
            }
            void SSD1308::setPages(uint8_t rowmin, uint8_t rowmax)
            {
                port->beginTransmission();
                    port->send(COMMAND_MODE);
                    port->send(SET_PAGE_ADDRESS);
                    port->send(rowmin); //set page address
                    port->send(rowmax); //set page address
                port->endTransmission();
            }
            void SSD1308::setCols(uint8_t colmin, uint8_t colmax)
            {
                port->beginTransmission();
                    port->send(COMMAND_MODE);
                    port->send(SET_COLUMN_ADDRESS);
                    port->send(colmin);
                    port->send(colmax);
                port->endTransmission();
            }
        }
    }
}
