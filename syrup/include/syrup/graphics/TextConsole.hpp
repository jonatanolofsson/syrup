#ifndef FRAMEBUFFER_TextConsole_HPP_
#define FRAMEBUFFER_TextConsole_HPP_

#include <stddef.h>
#include <stdint.h>
#include <syrup/graphics/Framebuffer.hpp>
#include <syrup/stream/BaseOutStream.hpp>
#include <syrup/stream/formatting.hpp>

namespace syrup {
    class TextConsole : public BaseOutStream
    {
        private:
            uint8_t row, col;
            uint8_t monospace;
            Framebuffer* fb;
            const unsigned char* font_ptr;
            unsigned int font_len;
            uint8_t font_height;
            uint8_t font_stride;
            uint8_t font_base;
            uint8_t font_max;
            const unsigned char* font_data_ptr;

        public:
            explicit TextConsole(Framebuffer* fb_, const uint8_t* font = 0);
            TextConsole();
            TextConsole(const TextConsole& other);
            TextConsole& operator=(const TextConsole& other);
            void setFont(const uint8_t* font);
            void setRow(uint8_t row_);
            void setCol(uint8_t col_);
            void setPos(uint8_t x, uint8_t y);
            void setMonospace(uint8_t width);
            void newline();
            void clear();

            BaseOutStream& flush();
            BaseOutStream& put(char_type ch); ///< Put char, advancing column and automatic line wrapping
            BaseOutStream& write(const char_type* s, size_type count);
            uint8_t putChar(char_type ch); ///< Low level put char function without advancing column.
            uint8_t getWidth(char_type ch);

            /// Text using a bitmap font
            TextConsole& textAt(const uint8_t x0, const uint8_t y0, size_t length, const unsigned char* str);
    };
}

#endif // FRAMEBUFFER_TextConsole_HPP_
